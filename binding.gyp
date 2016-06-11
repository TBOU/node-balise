{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "src/addon.cc",
        "src/utils.cc",
        "src/global.cc",
        "src/baliseprocess.cc"
      ],
      "include_dirs": ["sdk/include"],
      "conditions": [
        ["OS=='mac'", {
          "include_dirs": ["sdk/include/_darwin_x64"],
          "libraries": ["-L<!@(echo $BALISE_HOME)/bin", "-lubal"]
        }],
        ["OS=='linux'", {
          "include_dirs": ["sdk/include/_linux_x64"],
          "libraries": ["-luxbal"]
        }],
        ["OS=='win'", {
          "include_dirs": ["sdk/include/_windows_x64"],
          "libraries": ["sdk/lib/_windows_x64/libuxbal.lib"]
        }]
      ]
    }
  ]
}
