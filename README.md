# @lastapp/node-printer

Native printer bindings for **Node.js** and **Electron**. Enumerate printers, inspect and control print jobs, and send raw or file-based jobs directly to a printer from JavaScript — on Windows (Winspool) and POSIX/macOS ([CUPS](https://www.cups.org/)).

[![npm version](https://badge.fury.io/js/@lastapp%2Fnode-printer.svg)](https://www.npmjs.com/package/@lastapp/node-printer) [![Prebuild Binaries and Publish](https://github.com/last-tech/node-printer/actions/workflows/prebuild-main.yml/badge.svg)](https://github.com/last-tech/node-printer/actions/workflows/prebuild-main.yml)

> Maintained by **[Last.app](https://last.app)** ([last-tech](https://github.com/last-tech)). This is a hard fork of [`printer`](https://github.com/tojocky/node-printer) by **Ion Lupascu**, building on later community work by [@thiagoelg](https://github.com/thiagoelg) (Node 12+ support) and [@ekoeryanto](https://github.com/ekoeryanto) (prebuild + CI). We keep it current with modern Node and Electron and ship prebuilt binaries.

## Supported runtimes

Prebuilt binaries are published for Windows (`x64` and `ia32`). On other platforms the module is compiled from source at install time (see [Building from source](#building-from-source)).

| Runtime | Versions with prebuilt binaries |
|---------|---------------------------------|
| Node.js | 22, 24, 26 |
| Electron | 29, 31–43 |

The minimum supported Node.js version is **22.0.0** (`engines.node >= 22.0.0`). ABI is constant across an Electron major, so any patch release of a listed major is covered.

## Installation

```sh
npm install @lastapp/node-printer
# or
yarn add @lastapp/node-printer
```

`prebuild-install` fetches a matching prebuilt binary for your platform and runtime. If none is available, it falls back to a source build via `node-gyp`.

## Usage

```js
const printer = require('@lastapp/node-printer');

// List installed printers with their jobs and status
console.log(printer.getPrinters());

// Send a raw job to a specific printer
printer.printDirect({
  data: 'Hello world',
  printer: 'My_Printer',
  type: 'RAW',
  success: (jobId) => console.log('sent, job id:', jobId),
  error: (err) => console.error(err),
});
```

See the [`examples`](https://github.com/last-tech/node-printer/tree/main/examples) directory for more, and [`types/index.d.ts`](https://github.com/last-tech/node-printer/blob/main/types/index.d.ts) for the full typed API.

## API

| Method | Description |
|--------|-------------|
| `getPrinters()` | Enumerate all installed printers with their current jobs and statuses. |
| `getPrinter(printerName)` | Get a specific (or default) printer's info, jobs and status. |
| `getPrinterDriverOptions(printerName)` | Get a printer's driver options such as supported paper sizes. *(POSIX only)* |
| `getSelectedPaperSize(printerName)` | Get a printer's default paper size from its driver options. *(POSIX only)* |
| `getDefaultPrinterName()` | Return the default printer name. |
| `printDirect(options)` | Send a job to a printer. Supports [CUPS options](https://www.cups.org/doc/options.html) passed as a JS object. |
| `printFile(options)` | Print a file. *(POSIX only)* |
| `getSupportedPrintFormats()` | List supported print formats. `RAW` and `TEXT` are supported on all OSes. |
| `getJob(printerName, jobId)` | Get a specific job's info, including status. |
| `setJob(printerName, jobId, command)` | Send a command to a job (e.g. `'CANCEL'`). |
| `getSupportedJobCommands()` | List supported job commands. `'CANCEL'` is supported on all OSes. |

To print a PDF on Windows, convert it to EMF first (e.g. with [node-pdfium](https://github.com/tojocky/node-pdfium)) and send it as EMF.

## Building from source

A source build runs automatically when no prebuilt binary matches your platform/runtime. Requirements:

- A C++20 toolchain
  - **Windows**: Visual Studio with the *Desktop development with C++* workload
  - **macOS**: Xcode Command Line Tools
  - **Linux**: `build-essential`
- **POSIX only**: CUPS development headers (`libcups2-dev` on Debian/Ubuntu; bundled on macOS)
- Python 3 (for `node-gyp`)

```sh
npm run rebuild
```

## License

[MIT](https://opensource.org/licenses/MIT)

- Original author: Ion Lupascu (`ionlupascu@gmail.com`)
- Contributors: [@thiagoelg](https://github.com/thiagoelg), [@ekoeryanto](https://github.com/ekoeryanto)
- Maintained by [Last.app](https://last.app)
