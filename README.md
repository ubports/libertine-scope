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

To test the built scope, it needs to be be installed into a staging area first.
That's because the Unity test tools have rigid assumptions about the layout of various
installed data files, and the division between the sources and the built
artifacts that the CMake toolchain delivers do not meet those assumptions.  SO
be it:  installing into a staging area is no great hassle, but if you forget to
do it in your compile/install/run cycle you will meet with frustration.

Without system installation, you can test the scope using the unity-scope-tool
package available in the Ubuntu archives.

Here's the typical build/install/test cycle.

    make install DESTDIR=/tmpstaging/
    unity-scope-tool /tmp/staging/usr/local/lib/unity-scopes/libertine-scope/libertine-scope.ini

