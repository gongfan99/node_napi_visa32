{
  "targets": [
    {
      "target_name": "node-napi-visa32",
      "include_dirs": [ "/src" ],
      "sources": [ "src/addon.cc" ],
      "libraries": [ "<(module_root_dir)/src/visa64" ],
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1
        }
      },
      "conditions": [
        ["OS=='win'", {
          "defines": [
            "_HAS_EXCEPTIONS=1"
          ]
        }]
      ]
    }
  ]
}