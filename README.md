Libertine Scope
===============

The Libertine Scope is a Unity Scope that surfaces application launchers
contained in Libertine sandboxes.

Building the Scope
------------------

See the file debian/control for a list of required build dependencies.

Once the build dependencies are installed, all you should need to do to build
the project is the following set of commands.

    mkdir build && cd build
    cmake ..
    make

Testinmg the Scope
------------------

Without system installation, you can test the scope using the unity-scope-tool
package available in the Ubuntu archives.

    unity-scope-tool build/scope/apps/libertine-scope.ubuntu_libertine-scope.ini
