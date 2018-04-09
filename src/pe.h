#ifndef __PE_H
#define __PE_H

#include "semblance.h"

#pragma pack(1)

struct file_header {
    word  Machine;                      /* 04 */
    word  NumberOfSections;             /* 06 */
    dword TimeDateStamp;                /* 08 */
    dword PointerToSymbolTable;         /* 0c */
    dword NumberOfSymbols;              /* 10 */
    word  SizeOfOptionalHeader;         /* 14 */
    word  Characteristics;              /* 16 */
};

struct directory {
    dword address;
    dword size;
};

struct optional_header {
    /* Standard COFF fields. */
    word  Magic;                        /* 18 */
    byte  MajorLinkerVersion;           /* 1a */
    byte  MinorLinkerVersion;           /* 1b */
    dword SizeOfCode;                   /* 1c */
    dword SizeOfInitializedData;        /* 20 */
    dword SizeOfUninitializedData;      /* 24 */
    dword AddressOfEntryPoint;          /* 28 */
    dword BaseOfCode;                   /* 2c */
    dword BaseOfData;                   /* 30 */

    /* PE fields. */
    dword ImageBase;                    /* 34 */
    dword SectionAlignment;             /* 38 */
    dword FileAlignment;                /* 3c */
    word  MajorOperatingSystemVersion;  /* 40 */
    word  MinorOperatingSystemVersion;  /* 42 */
    word  MajorImageVersion;            /* 44 */
    word  MinorImageVersion;            /* 46 */
    word  MajorSubsystemVersion;        /* 48 */
    word  MinorSubsystemVersion;        /* 4a */
    dword Win32VersionValue;            /* 4c */
    dword SizeOfImage;                  /* 50 */
    dword SizeOfHeaders;                /* 54 */
    dword CheckSum;                     /* 58 */
    word  Subsystem;                    /* 5c */
    word  DllCharacteristics;           /* 5e */
    dword SizeOfStackReserve;           /* 60 */
    dword SizeOfStackCommit;            /* 64 */
    dword SizeOfHeapReserve;            /* 68 */
    dword SizeOfHeapCommit;             /* 6c */
    dword LoaderFlags;                  /* 70 */
    dword NumberOfRvaAndSizes;          /* 74 */
};

struct header_pe {
    dword Signature;                    /* 00 */

    struct file_header file;            /* 04 */
    struct optional_header opt;         /* 18 */
};

STATIC_ASSERT(sizeof(struct header_pe) == 0x78);

struct section {
    char  name[8];          /* 00 */
    dword min_alloc;        /* 08 */
    dword address;          /* 0c */
    dword length;           /* 10 */
    dword offset;           /* 14 */
    dword reloc_offset;     /* 18 */
    dword lineno_offset;    /* 1c */
    word  reloc_count;      /* 20 */
    word  lineno_count;     /* 22 */
    dword flags;            /* 24 */

    /* and our data: */
    byte *instr_flags;
};

struct reloc_pe {
    unsigned int offset:12;
    unsigned int type:4;
};

STATIC_ASSERT(sizeof(struct reloc_pe) == 0x2);

#pragma pack()

struct export {
    dword address;
    word ordinal;
    char *name;
};

struct import_module {
    char *module;
    dword nametab_addr;
    char **nametab;
    unsigned count;
};

struct pe {
    struct header_pe header;
    struct directory *dirs;

    char *name;

    struct section *sections;

    struct export *exports;
    unsigned export_count;

    struct import_module *imports;
    unsigned import_count;

    dword reloc_base;
    struct reloc_pe *relocs;
    unsigned reloc_count;
};

/* in pe_section.c */
extern struct section *addr2section(dword addr, const struct pe *pe);
extern long addr2offset(dword addr, const struct pe *pe);
extern void read_sections(struct pe *pe);
extern void print_sections(struct pe *pe);

#endif /* __PE_H */
