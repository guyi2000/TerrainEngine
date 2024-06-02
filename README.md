# Terrain Engine

## 演示视频

https://github.com/guyi2000/TerrainEngine/assets/16850027/af0a0626-a7dc-443a-996c-9899e0b4db94

## SkyBox

- 没有使用Cubemap，原因是底面为水，纹理的重复模式不同
- 绑定一个VAO、VBO、EBO，对五个面进行旋转方法进行绘制
- 封装为一个类中，完成天空盒的Stretch功能
- 使用`GL_CLAMP_TO_EDGE`完成边缘消去

## Water wave

- Water使用新的VAO、VBO、EBO，纹理使用`GL_REPEAT`重复
- 通过改变纹理坐标完成水的流动效果

## Terrain model

- 读取`heightmap.bmp`，注意其为3通道8位灰度图，所以可以间隔取值，并将值映射到高度所在范围
- 以`heightmap.bmp`为分辨率生成`vertices`和`indices`用于VAO、VBO、EBO

```C++
unsigned char* data = stbi_load("data/heightmap.bmp", &width, &height, &nrComponents, 0);
for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
        vertices.push_back((float)j / height);
        vertices.push_back((float)(*(data + 3 * (i * width + j))) / 1500.0f);
        vertices.push_back((float)i / width);
        vertices.push_back((float)j / height);
        vertices.push_back((float)i / width);
    }
}
for (int i = 0; i < height - 1; i++) {
    for (int j = 0; j < width - 1; j++) {
        indices.push_back(i * width + j);
        indices.push_back(i * width + j + 1);
        indices.push_back((i + 1) * width + j);
        indices.push_back(i * width + j + 1);
        indices.push_back((i + 1) * width + j + 1);
        indices.push_back((i + 1) * width + j);
    }
}
```

- 纹理坐标映射后，注意此处使用两个纹理，一个是`terrain-texture3.bmp`另一个是`detail.bmp`
- 纹理坐标的混合使用`SRC1+SRC2-0.5`的形式
- 由于detail需要重复，因此在片段着色器中将其`*10`，表示每个方向重复10次

## Reflection

- 反射实现主要是将Water改为半透明，然后重新绘制一个倒的天空盒与Terrain Model
- 具体可以修改顶点着色器，将y坐标反转并做简单的坐标变换

## Interaction

- 使用`camera.hpp`完成，同之前作业，增加了UP与DOWN的行为

### 操作指南

- <kbd>W</kbd>: 前进
- <kbd>S</kbd>: 后退
- <kbd>A</kbd>: 向左
- <kbd>D</kbd>: 向右
- <kbd>Space</kbd>: 上升
- <kbd>Q</kbd>: 下降
- 鼠标移动：视角变换
- 鼠标滚轮：放大缩小
