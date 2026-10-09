# Legacy engine source

Verbatim copies of the Kismo engine source from the historical archives, imported on 2026-10-09. Nothing here is built by this repo yet. The current engine layout is decided separately.

## Layout

One directory per archive tree, keeping the tree's own relative paths:

| Directory | Archive | Source dates |
|---|---|---|
| `kismo-core/` | `\\crogserver\Development\C++\KismoCore` | 2009-2017 |
| `kismo-utility/` | `_ARCHIVE_\Kismo Utility` | 2010-2011 |
| `kismo-v0.4/` | `_ARCHIVE_\Kismo v0.4` | 2003-2010 |
| `kismo-v0.3/` | `_ARCHIVE_\Kismo v0.3` | 2000-2010 |
| `rage-demo/` | `_ARCHIVE_\RAGE_Demo` | 2000-2008 |
| `kismo-0.2/` | `_ARCHIVE_\Kismo-0.2` | 2005-2008 |

`MANIFEST.csv` lists every imported file with its original path, size, SHA-256, and status.

## What is included

Source files only: `.h`, `.hpp`, `.inl`, `.c`, `.cpp`, `.cc`, `.cxx`, `.rc`, `.def`, `.mel`, `.ui`, `.qrc`.

## What is left out

- Project and solution files (`.vcproj`, `.vcxproj`, `.sln`, `.filters`, `.user`, `.suo`, `.aps`, `.dsp`, and similar). They are build configuration, not source.
- Build output and intermediate directories (`Debug`, `Release`, `x64`, `x86`, `Win32`, `obj`, `bin`, `ipch`, and the `Debug Metrics`/`Release Metrics` logs).
- Generated and binary output: `.html`, `.png`, `.md5`, `.map`, `.obj`, `.dll`, `.lib`, `.exe`, `.pdb`, and similar.
- Vendored third-party code: GLEW, glut, and RapidXML. None of it is Kismo work, so none of it is imported.

Excluded files are not in this repo. The archive shares remain the only copy.

## Header format

Each imported file starts with a provenance comment. The file body below it is byte-for-byte the original.

- `Origin` is the full share path of the file.
- `Status` is one of:
  - `current`: the newest copy of this file name in the archive.
  - `unique`: no other copy of this file name in the archive.
  - `duplicate of <path>`: another tree holds a copy with identical content.
  - `superseded by <path> (same name, content differs)`: a copy with the same file name in a newer tree has different content.

`superseded` is a file-name and content comparison only. It does not record whether the author intended the newer file to replace the older one, or whether the two are unrelated files that happen to share a name. Check the code before relying on it.

## Verification

`MANIFEST.csv` records each original's SHA-256. The import check strips each header and compares the remaining bytes with the original (minus any UTF-8 byte-order mark, which is kept at the start of the file). All 1,025 files passed.

`.gitattributes` sets `legacy/** -text`, so git stores these bytes unchanged: no line-ending or encoding conversion.
