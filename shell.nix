{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    qt6.qtbase
    qt6.qtdeclarative
    qt6.qtquicktimeline
    qt6.qtshadertools
    qt6.qttools
    qt6.qtwayland
    kdePackages.qtdeclarative


    cmake
    ninja
    clang
    gdb
    curl
    nlohmann_json
    gtest
    lldb
    cmark-gfm

     qt6.qtsvg
     qt6.qttools
     qt6.qtimageformats
     kdePackages.qtmultimedia
     kdePackages.qtshadertools
     kdePackages.syntax-highlighting
     qt6.qtmultimedia
  ];

  shellHook = ''
    export QT_QPA_PLATFORM=wayland

    export QML2_IMPORT_PATH=${pkgs.qt6.qtdeclarative}/lib/qt-6/qml
    export QT_PLUGIN_PATH=${pkgs.qt6.qtbase}/lib/qt-6/plugins

    export PKG_CONFIG_PATH=${pkgs.cmark-gfm}/lib/pkgconfig:$PKG_CONFIG_PATH
  '';
}