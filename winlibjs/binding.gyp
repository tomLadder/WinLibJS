{
  "targets": [
    {
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
         "include", "C:\\Dev\\WinLib\\WinLib\\WinLib"
      ],
      "target_name": "winlib",
      "sources": [ "./src/mmap.cpp" ],
      "libraries": [
        "C:\\Dev\\WinLib\\WinLib\\x64\\Release\\WinLib.lib"
      ],
    }
  ]
}