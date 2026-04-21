# Storage Layer API (v1)

This document defines the public storage interfaces used by all higher-level modules (`B+Tree`, `TableHeap`, `Executor`, etc.).

The goal of this API is to provide:

- Safe page lifetime management
- Stable interfaces for future internal upgrades
- Separation of storage internals from higher layers
- Compatibility with future concurrency and performance optimizations

---

# Modules

Storage layer currently consists of:

- `DiskManager`
- `Page`
- `BufferPoolManager`
- `ReadPageGuard`
- `WritePageGuard`

Higher layers must interact through `BufferPoolManager`.

---

# Layering Rules

Allowed dependency flow:

```text
Executor / Parser
        ↓
TableHeap / B+Tree
        ↓
BufferPoolManager
        ↓
DiskManager
        ↓
Database File
```

Forbidden:

- `B+Tree` directly reading files
- `Executor` using `DiskManager`
- Higher layers manually managing page pin counts
- Higher layers touching internal metadata (`dirty`, `pin_count`, etc.)

---

# Core Concepts

## Page

A page is a fixed-size block of bytes (`PAGE_SIZE`, currently 4096 bytes).

A page may exist:

- On disk (persistent)
- In buffer pool memory (cached)

---

## Guard Objects

Pages are accessed through RAII guards.

Guards automatically release page resources when destroyed.

Benefits:

- No manual `UnpinPage()`
- No forgotten cleanup
- Safer ownership model

---

# Public BufferPoolManager API

```cpp
ReadPageGuard  FetchPageRead(page_id_t page_id);
WritePageGuard FetchPageWrite(page_id_t page_id);
WritePageGuard NewPage();

bool DeletePage(page_id_t page_id);

bool FlushPage(page_id_t page_id);
void FlushAllPages();
```

---

# Read Access

Use when page contents are only inspected.

```cpp
auto guard = bpm.FetchPageRead(page_id);
const char* data = guard.GetData();
```

Properties:

- Read-only access
- Automatically released when guard goes out of scope

---

# Write Access

Use when page contents may be modified.

```cpp
auto guard = bpm.FetchPageWrite(page_id);
char* data = guard.GetData();
```

Properties:

- Writable access
- Automatically marked dirty on guard destruction
- Automatically released when guard goes out of scope

---

# Creating New Pages

```cpp
auto guard = bpm.NewPage();
page_id_t id = guard.GetPageId();
```

Properties:

- Returns writable empty page
- New page receives unique page id
- Automatically managed like normal write page

---

# Deleting Pages

```cpp
bool ok = bpm.DeletePage(page_id);
```

Behavior:

- Fails if page is currently pinned/in use
- Removes page from memory if present
- Releases page id back to storage manager

---

# Flushing Pages

## Single Page

```cpp
bpm.FlushPage(page_id);
```

Writes page to disk if dirty.

## All Pages

```cpp
bpm.FlushAllPages();
```

Writes all dirty pages to disk.

Recommended during shutdown.

---

# Safety Rules

## Rule 1: Never Keep Raw Pointers After Guard Destruction

Wrong:

```cpp
char* ptr;
{
    auto guard = bpm.FetchPageWrite(id);
    ptr = guard.GetData();
}
ptr[0] = 'x';
```

Correct:

```cpp
{
    auto guard = bpm.FetchPageWrite(id);
    char* ptr = guard.GetData();
    ptr[0] = 'x';
}
```

---

## Rule 2: Use Small Guard Scope

Prefer:

```cpp
{
    auto guard = bpm.FetchPageRead(id);
    ...
}
```

Avoid holding guards longer than necessary.

---

## Rule 3: Use Correct Access Type

Use:

- `FetchPageRead()` for reading
- `FetchPageWrite()` for modifying

Do not request write access unless needed.

---

## Rule 4: Do Not Modify Through Read Guards

`ReadPageGuard` returns:

```cpp
const char*
```

Bytes are read-only.

---

# Error Handling

If page cannot be fetched or allocated:

- Guard may be invalid

Check:

```cpp
auto guard = bpm.FetchPageRead(id);
if (!guard.IsValid()) {
    ...
}
```

---

# Threading Compatibility (Future)

This API is intentionally designed so future concurrency features can be added without changing callers.

Potential future internal additions:

- Shared page latches in `ReadPageGuard`
- Exclusive page latches in `WritePageGuard`
- Buffer pool mutexes
- Background flushing
- Prefetching

Callers should continue using same API.

---

# Performance Compatibility (Future)

Internal implementations may change without API changes:

- LRU → CLOCK replacement
- Async flush batching
- Hot page caching
- Metrics
- Page compression

---

# Examples

## B+Tree Search

```cpp
auto root = bpm.FetchPageRead(root_page_id);
const char* node = root.GetData();
```

## B+Tree Insert

```cpp
auto leaf = bpm.FetchPageWrite(leaf_page_id);
char* node = leaf.GetData();
```

## Table Row Insert

```cpp
auto page = bpm.FetchPageWrite(data_page_id);
char* bytes = page.GetData();
```

---

# Design Philosophy

This API favors:

- correctness by construction
- RAII ownership
- explicit read/write intent
- stable contracts
- hidden internal complexity

---

# Versioning

Current version:

```text
Storage API v1
```

Future breaking changes should increment version.

