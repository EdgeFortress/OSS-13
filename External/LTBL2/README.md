# Let There Be Light 2

A 2D dynamic soft shadows system with accurate penumbras/antumbras.

This is a modified version of LTBL2 by Cmdu76

Install
-------

LTBL2 relies only on [SFML 2.4.0](http://www.sfml-dev.org/download/sfml/2.4.0/index.php)

Add LetThereBeLight.hpp & LetThereBeLight.cpp to your project

Quick Start
-----------

- [See the example](examples/main.cpp)
- [See the documentation](https://cmdu76.github.io/LTBL2)

Changes
-------

- Light management : use create...() to create your light and add it to the system (no shared pointers used)
- Quadtree : rebuilt entirely
- LightSystem : simplified render call and minor performance improvements
- LightShape & LightPointEmission are updated with the quadtree
- LightDirectionEmission has been simplified
- Remove/change some hardcoded value
- Add resources direclty in C++ (PenumbraTexture, UnshadowShader & LightOverShapeShader)
- Documentation
- Header-only version

License
-------

[See it here](LICENSE.md)