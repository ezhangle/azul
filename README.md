# ![Icon](https://3d.bk.tudelft.nl/ken/img/azul-small.png) azul

azul is a 3D viewer for macOS 10.13. It is intended for viewing 3D city models in (City)GML, CityJSON, OBJ, OFF and POLY. It supports loading multiple files, selecting objects by clicking them or selecting them in the sidebar, and browsing their attributes. It is currently pre-release software, but it is pretty stable and most datasets already work without problems. It is available under the GPLv3 licence.

## Controls

* Pan: scroll
* Rotate: drag on (left) click, rotate on trackpad
* Zoom: pinch on trackpad, drag on right click
* Select: click on object (in view or sidebar)
* Centre view: double click (in view or sidebar object), h or cmd+shift+h (to dataset centre)

* New file (clear view): n or cmd+n
* Open file (import): o or cmd+o
* Load view (camera) parameters: l or cmd+l
* Save view (camera) parameters: s or cmd+s
* Copy selected object ids: c or cmd+c
* Find objects: f or cmd+f
* Show bounding box: b or cmd+shift+b
* Show edges: e or cmd+shift+e

* Show sidebar: cmd+ctrl+s
* Go full screen: cmd+ctrl+f
* Close window (quits): cmd+w
* Quit: cmd+q

## Download

You can download the latest stable release of azul in the [releases page](https://github.com/tudelft3d/azul/releases) or from the [App Store](https://itunes.apple.com/app/azul/id1173239678?mt=12). If you want more information on how to compile it from source, see below.

![Random3DCity](https://3d.bk.tudelft.nl/ken/img/azul/random.png)

![LOD2 example](https://3d.bk.tudelft.nl/ken/img/azul/lod2.png)

![The Hague](https://3d.bk.tudelft.nl/ken/img/azul/denhaag.png)

![CityGML 2.0 sample dataset](https://3d.bk.tudelft.nl/ken/img/azul/citygml2.png)

![Ettenheim](https://3d.bk.tudelft.nl/ken/img/azul/ettenheim.png)

![New York City](https://3d.bk.tudelft.nl/ken/img/azul/nyc.png)

## Technical details

azul is written in a mix of C++14, Swift 4, Objective-C 2 and Objective-C++. The core is written in C++, but it uses Apple's Metal for visualisation and SIMD for fast vector/matrix computations. It uses [pugixml](https://pugixml.org) in order to parse XML, Niels Lohmann's [JSON for Modern C++](https://github.com/nlohmann/json) library, and the [CGAL](https://www.cgal.org) Triangulation package to triangulate concave polygons for display.

## Not implemented / ideas for the future

* Removing (unloading) files
* Toggle to hide objects
* Improved search with live viewing of matching objects
* Customising colours, more complex materials
* Materials and textures (loaded from files)
* Implicit representations
* Shifting the rotation point out of the data plane
* Using a rotation point at a visible object in the centre (good for zooming in and rotating)
* Showing the data plane and rotation point
* Animations when re-centering
* Keyboard navigation
* Multi-threaded file loading and ray shooting
* QuickLook plug-in
* Icon previews
* iOS support

## Compilation

We have included an Xcode 9 project to easily compile azul. Note that older versions of Xcode cannot compile Swift 4.

azul depends on the following libraries: [Boost](http://www.boost.org), [CGAL](http://www.cgal.org), [GMP](https://gmplib.org), [MPFR](http://www.mpfr.org) and [pugixml](http://pugixml.org). These can be easily obtained using [Homebrew](http://brew.sh), but we provide bundled static versions of them for convenience.

## Licence

azul is available under the [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html) licence.
