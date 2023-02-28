# canvas-drawer

Implements a simple drawing api

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake -G "Visual Studio 16 2019" ..
canvas-drawer/build $ start Draw-2D.sln
```

Your solution file should contain two projects: `pixmap_art` and `pixmap_test`.
To run from the git bash command shell, 

```
canvas-drawer/build $ ../bin/Debug/draw_test
canvas-drawer/build $ ../bin/Debug/draw_art
```

*macOS*

Open terminal to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake ..
canvas-drawer/build $ make
```

To run each program from build, you would type

```
canvas-drawer/build $ ../bin/draw_test
canvas-drawer/build $ ../bin/draw_art
```

## Results


![triangle](https://user-images.githubusercontent.com/44120884/221911857-60408f09-fb6d-47c7-8f93-40176644ad48.png)


![quad](https://user-images.githubusercontent.com/44120884/221911913-b5d33be7-744b-4d87-87c4-1043808244fe.png)


![line-color-interpolation](https://user-images.githubusercontent.com/44120884/221912050-c9eb6cab-fa5f-49ff-b541-d44caa41d050.png)


![diagonal-line-2](https://user-images.githubusercontent.com/44120884/221912116-9881c612-dcb7-433a-b635-2abeb0f5d3fa.png)


![vertical-line](https://user-images.githubusercontent.com/44120884/221912227-6b8e25a2-9669-4222-aa3f-8ec86514acf8.png)


![horizontal-line](https://user-images.githubusercontent.com/44120884/221912265-9730690e-d6b2-4efe-92a9-02a4280d527c.png)


