# shell.nix
{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.curl
    pkgs.gcc
    pkgs.gdb
    pkgs.clang-tools
    pkgs.concord
    pkgs.sqlite
    pkgs.sqlitebrowser
  ];

  shellHook = ''zsh'';
}

