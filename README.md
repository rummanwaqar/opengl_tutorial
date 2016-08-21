## To compile

```
mkdir build && cd $_
cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../opengl_tutorial
```
Import the project into eclipse.

## Dependencies

* Texture loading requires the soil library `libsoil-dev`
* Transformations requires the glm library `libglm-dev`
