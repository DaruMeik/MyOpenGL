# MyOpenGL
OpenGLを学習するを目標をして、様々なことを試した。Buildのフォルダの.exeファイルで実行する。

## MyFlappyBird
OpenGLを通じてFlappyBirdというゲームを作ってみた。ゲームエンジンとは言えないが、ゲームプログラミングの基本的な機能（ゲームオブジェクトの生成を削除や、メモリ管理や衝突検査など）を搭載した。

![MyFlappyBird](https://github.com/user-attachments/assets/b24241c6-ff50-446c-8a85-07f227de0140)


## MyRayTracer
ピクセルシェーダを通じてリアルタイムでRayTracingのレンダリングすることを目標である。

### Base Raymarching
Raytracingより複雑な形があるオブジェクトに向いているRayMarchingSDFを利用してレンダリングするシーン。オブジェクトのシェーダはBlinnPhongのモデルである。

![BaseRaymarching](https://github.com/user-attachments/assets/c2d72338-60e3-4214-ac29-10860a9e14ca)


### Bouncing Raymarching
RayMarchingSDFで、例を反射させたり、屈折させたりするようにレンダリングするシーン。Raytracingとかなり似ている仕組みのため、光と影はRaytracingと同じ方法で模倣できた。

![BouncingRaymarching](https://github.com/user-attachments/assets/3983fb4d-6019-48cc-a960-46e1970baaa0)

## 参考文献
https://raytracing.github.io/books/RayTracingInOneWeekend.html

https://iquilezles.org/articles/raymarchingdf/

https://michaelwalczyk.com/blog-ray-marching.html
