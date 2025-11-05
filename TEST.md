Hi. On Linux, when compiling Python with ASAN using:

```bash
./configure --prefix=NAME --enable-pystats --without-pymalloc --with-pydebug --with-address-sanitizer --with-undefined-behavior-sanitizer
```

and then running CFFI in any API mode, we encounter memory leaks.

```
CFFI-MemoryLeak/src/API-MainMode$ pydebug build.py 

generating ./_pi_cffi.c
the current directory is 'CFFI-MemoryLeak/src/API-MainMode'


CFFI-MemoryLeak/src/API-MainMode$ pydebug test.py 
70.71067811865476

=================================================================
==63951==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 208 byte(s) in 1 object(s) allocated from:
    #0 0x7c95ac6fd9c7 in malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x5e4b61d5448d in _PyMem_RawMalloc Objects/obmalloc.c:45
    #2 0x5e4b61d55e7a in _PyMem_DebugRawAlloc Objects/obmalloc.c:2096
    #3 0x5e4b61d55f91 in _PyMem_DebugRawMalloc Objects/obmalloc.c:2129
    #4 0x5e4b61d55fbb in _PyMem_DebugMalloc Objects/obmalloc.c:2281
    #5 0x5e4b61d5a3c0 in PyObject_Malloc Objects/obmalloc.c:801
    #6 0x5e4b6224879f in gc_alloc Modules/gcmodule.c:2330
    #7 0x5e4b62248b1b in _PyObject_GC_New Modules/gcmodule.c:2345
    #8 0x7c95ab249025 in ffi_internal_new (/home/kotboris/pythonExp/lib/python3.12/site-packages/_cffi_backend.cpython-312-x86_64-linux-gnu.so+0x23025) (BuildId: 5b2225c347aec0c67666bb29c5ea7330b09c5fe5)
    #9 0x7c95ab250094 in b_init_cffi_1_0_external_module (/home/kotboris/pythonExp/lib/python3.12/site-packages/_cffi_backend.cpython-312-x86_64-linux-gnu.so+0x2a094) (BuildId: 5b2225c347aec0c67666bb29c5ea7330b09c5fe5)
    #10 0x5e4b61d3c763 in cfunction_vectorcall_O Objects/methodobject.c:509
    #11 0x5e4b61c14584 in _PyObject_VectorcallTstate Include/internal/pycore_call.h:92
    #12 0x5e4b61c14f19 in _PyObject_CallFunctionVa Objects/call.c:562
    #13 0x5e4b61c1594e in callmethod Objects/call.c:634
    #14 0x5e4b61c15bf7 in PyObject_CallMethod Objects/call.c:653
    #15 0x7c95abc1c810 in _cffi_init /home/kotboris/PYSATL/CFFI-MemoryLeak/src/API-MainMode/_pi_cffi.c:434
    #16 0x7c95abc1c91d in PyInit__pi_cffi /home/kotboris/PYSATL/CFFI-MemoryLeak/src/API-MainMode/_pi_cffi.c:661
    #17 0x5e4b6211b071 in _PyImport_LoadDynamicModuleWithSpec Python/importdl.c:169
    #18 0x5e4b62115fd2 in _imp_create_dynamic_impl Python/import.c:3789
    #19 0x5e4b621160b7 in _imp_create_dynamic Python/clinic/import.c.h:506
    #20 0x5e4b61d3c585 in cfunction_vectorcall_FASTCALL Objects/methodobject.c:422
    #21 0x5e4b61c18d5f in _PyVectorcall_Call Objects/call.c:271
    #22 0x5e4b61c19914 in _PyObject_Call Objects/call.c:354
    #23 0x5e4b61c199ab in PyObject_Call Objects/call.c:379
    #24 0x5e4b620333d6 in _PyEval_EvalFrameDefault Python/bytecodes.c:3263
    #25 0x5e4b6203dee1 in _PyEval_EvalFrame Include/internal/pycore_ceval.h:89
    #26 0x5e4b6203e212 in _PyEval_Vector Python/ceval.c:1685
    #27 0x5e4b61c13c25 in _PyFunction_Vectorcall Objects/call.c:419
    #28 0x5e4b61c14584 in _PyObject_VectorcallTstate Include/internal/pycore_call.h:92
    #29 0x5e4b61c170e2 in object_vacall Objects/call.c:850

Direct leak of 138 byte(s) in 1 object(s) allocated from:
    #0 0x7c95ac6fd9c7 in malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x5e4b61d5448d in _PyMem_RawMalloc Objects/obmalloc.c:45
    #2 0x5e4b61d55e7a in _PyMem_DebugRawAlloc Objects/obmalloc.c:2096
    #3 0x5e4b61d55f91 in _PyMem_DebugRawMalloc Objects/obmalloc.c:2129
    #4 0x5e4b61d55fbb in _PyMem_DebugMalloc Objects/obmalloc.c:2281
    #5 0x5e4b61d598eb in PyMem_Malloc Objects/obmalloc.c:703
    #6 0x7c95ab24c2cb in lib_build_cpython_func (/home/kotboris/pythonExp/lib/python3.12/site-packages/_cffi_backend.cpython-312-x86_64-linux-gnu.so+0x262cb) (BuildId: 5b2225c347aec0c67666bb29c5ea7330b09c5fe5)
    #7 0x7c95ab24c862 in lib_build_and_cache_attr (/home/kotboris/pythonExp/lib/python3.12/site-packages/_cffi_backend.cpython-312-x86_64-linux-gnu.so+0x26862) (BuildId: 5b2225c347aec0c67666bb29c5ea7330b09c5fe5)
    #8 0x7c95ab24d2fc in lib_getattr (/home/kotboris/pythonExp/lib/python3.12/site-packages/_cffi_backend.cpython-312-x86_64-linux-gnu.so+0x272fc) (BuildId: 5b2225c347aec0c67666bb29c5ea7330b09c5fe5)
    #9 0x5e4b61d50b0b in PyObject_GetAttr Objects/object.c:1060
    #10 0x5e4b61fe9661 in _PyEval_EvalFrameDefault Python/bytecodes.c:1802
    #11 0x5e4b6203dee1 in _PyEval_EvalFrame Include/internal/pycore_ceval.h:89
    #12 0x5e4b6203e212 in _PyEval_Vector Python/ceval.c:1685
    #13 0x5e4b6203e54e in PyEval_EvalCode Python/ceval.c:580
    #14 0x5e4b6219ea24 in run_eval_code_obj Python/pythonrun.c:1757
    #15 0x5e4b6219ec3b in run_mod Python/pythonrun.c:1778
    #16 0x5e4b621a21ff in pyrun_file Python/pythonrun.c:1674
    #17 0x5e4b621a6f6f in _PyRun_SimpleFileObject Python/pythonrun.c:459
    #18 0x5e4b621a7254 in _PyRun_AnyFileObject Python/pythonrun.c:78
    #19 0x5e4b62239014 in pymain_run_file_obj Modules/main.c:361
    #20 0x5e4b622392ee in pymain_run_file Modules/main.c:380
    #21 0x5e4b6223bfd3 in pymain_run_python Modules/main.c:634
    #22 0x5e4b6223c1db in Py_RunMain Modules/main.c:714
    #23 0x5e4b6223c3f0 in pymain_main Modules/main.c:744
    #24 0x5e4b6223c775 in Py_BytesMain Modules/main.c:768
    #25 0x5e4b61967425 in main Programs/python.c:15
    #26 0x7c95aba2a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #27 0x7c95aba2a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #28 0x5e4b61967354 in _start (/home/kotboris/pythonExp/bin/python3.12+0x1165354) (BuildId: 1677bd47c890dd8f14394ed77827d2f1c359d1f9)

Direct leak of 128 byte(s) in 1 object(s) allocated from:
    #0 0x7c95ac6fd9c7 in malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x5e4b61d5448d in _PyMem_RawMalloc Objects/obmalloc.c:45
    #2 0x5e4b61d55e7a in _PyMem_DebugRawAlloc Objects/obmalloc.c:2096
    #3 0x5e4b61d55f91 in _PyMem_DebugRawMalloc Objects/obmalloc.c:2129
    #4 0x5e4b61d55fbb in _PyMem_DebugMalloc Objects/obmalloc.c:2281
    #5 0x5e4b61d598eb in PyMem_Malloc Objects/obmalloc.c:703
    #6 0x7c95ab24feb4 in _my_Py_InitModule (/home/kotboris/pythonExp/lib/python3.12/site-packages/_cffi_backend.cpython-312-x86_64-linux-gnu.so+0x29eb4) (BuildId: 5b2225c347aec0c67666bb29c5ea7330b09c5fe5)
    #7 0x7c95ab25006c in b_init_cffi_1_0_external_module (/home/kotboris/pythonExp/lib/python3.12/site-packages/_cffi_backend.cpython-312-x86_64-linux-gnu.so+0x2a06c) (BuildId: 5b2225c347aec0c67666bb29c5ea7330b09c5fe5)
    #8 0x5e4b61d3c763 in cfunction_vectorcall_O Objects/methodobject.c:509
    #9 0x5e4b61c14584 in _PyObject_VectorcallTstate Include/internal/pycore_call.h:92
    #10 0x5e4b61c14f19 in _PyObject_CallFunctionVa Objects/call.c:562
    #11 0x5e4b61c1594e in callmethod Objects/call.c:634
    #12 0x5e4b61c15bf7 in PyObject_CallMethod Objects/call.c:653
    #13 0x7c95abc1c810 in _cffi_init /home/kotboris/PYSATL/CFFI-MemoryLeak/src/API-MainMode/_pi_cffi.c:434
    #14 0x7c95abc1c91d in PyInit__pi_cffi /home/kotboris/PYSATL/CFFI-MemoryLeak/src/API-MainMode/_pi_cffi.c:661
    #15 0x5e4b6211b071 in _PyImport_LoadDynamicModuleWithSpec Python/importdl.c:169
    #16 0x5e4b62115fd2 in _imp_create_dynamic_impl Python/import.c:3789
    #17 0x5e4b621160b7 in _imp_create_dynamic Python/clinic/import.c.h:506
    #18 0x5e4b61d3c585 in cfunction_vectorcall_FASTCALL Objects/methodobject.c:422
    #19 0x5e4b61c18d5f in _PyVectorcall_Call Objects/call.c:271
    #20 0x5e4b61c19914 in _PyObject_Call Objects/call.c:354
    #21 0x5e4b61c199ab in PyObject_Call Objects/call.c:379
    #22 0x5e4b620333d6 in _PyEval_EvalFrameDefault Python/bytecodes.c:3263
    #23 0x5e4b6203dee1 in _PyEval_EvalFrame Include/internal/pycore_ceval.h:89
    #24 0x5e4b6203e212 in _PyEval_Vector Python/ceval.c:1685
    #25 0x5e4b61c13c25 in _PyFunction_Vectorcall Objects/call.c:419
    #26 0x5e4b61c14584 in _PyObject_VectorcallTstate Include/internal/pycore_call.h:92
    #27 0x5e4b61c170e2 in object_vacall Objects/call.c:850
    #28 0x5e4b61c1742b in PyObject_CallMethodObjArgs Objects/call.c:911
    #29 0x5e4b62112ad4 in import_find_and_load Python/import.c:2793

Indirect leak of 88 byte(s) in 1 object(s) allocated from:
    #0 0x7c95ac6fd9c7 in malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x5e4b61d5448d in _PyMem_RawMalloc Objects/obmalloc.c:45
    #2 0x5e4b61d55e7a in _PyMem_DebugRawAlloc Objects/obmalloc.c:2096
    #3 0x5e4b61d55f91 in _PyMem_DebugRawMalloc Objects/obmalloc.c:2129
    #4 0x5e4b61d55fbb in _PyMem_DebugMalloc Objects/obmalloc.c:2281
    #5 0x5e4b61d5a3c0 in PyObject_Malloc Objects/obmalloc.c:801
    #6 0x5e4b6224879f in gc_alloc Modules/gcmodule.c:2330
    #7 0x5e4b62248b1b in _PyObject_GC_New Modules/gcmodule.c:2345
    #8 0x5e4b61cfc949 in new_dict Objects/dictobject.c:751
    #9 0x5e4b61cff9a6 in PyDict_New Objects/dictobject.c:854
    #10 0x5e4b61cff9f7 in dict_new_presized Objects/dictobject.c:1593
    #11 0x5e4b61d0bacb in _PyDict_FromItems Objects/dictobject.c:1636
    #12 0x5e4b61fe21a8 in _PyEval_EvalFrameDefault Python/bytecodes.c:1548
    #13 0x5e4b6203dee1 in _PyEval_EvalFrame Include/internal/pycore_ceval.h:89
    #14 0x5e4b6203e212 in _PyEval_Vector Python/ceval.c:1685
    #15 0x5e4b61c13c25 in _PyFunction_Vectorcall Objects/call.c:419
    #16 0x5e4b61c14584 in _PyObject_VectorcallTstate Include/internal/pycore_call.h:92
    #17 0x5e4b61c170e2 in object_vacall Objects/call.c:850
    #18 0x5e4b61c1742b in PyObject_CallMethodObjArgs Objects/call.c:911
    #19 0x5e4b62112ad4 in import_find_and_load Python/import.c:2793
    #20 0x5e4b62118b02 in PyImport_ImportModuleLevelObject Python/import.c:2876
    #21 0x5e4b61f88b7f in builtin___import___impl Python/bltinmodule.c:276
    #22 0x5e4b61f89018 in builtin___import__ Python/clinic/bltinmodule.c.h:107
    #23 0x5e4b61d3c073 in cfunction_vectorcall_FASTCALL_KEYWORDS Objects/methodobject.c:438
    #24 0x5e4b61c14584 in _PyObject_VectorcallTstate Include/internal/pycore_call.h:92
    #25 0x5e4b61c14f19 in _PyObject_CallFunctionVa Objects/call.c:562
    #26 0x5e4b61c1552a in PyObject_CallFunction Objects/call.c:584
    #27 0x5e4b6211936b in PyImport_Import Python/import.c:3062
    #28 0x5e4b6211947f in PyImport_ImportModule Python/import.c:2486
    #29 0x5e4b6217bfaa in init_import_site Python/pylifecycle.c:2317

SUMMARY: AddressSanitizer: 562 byte(s) leaked in 4 allocation(s).

```

An example demonstrating this issue is available in the [repository](https://github.com/K0tB0ris/CFFI-MemoryLeak.git)
