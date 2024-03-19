# 鼠标双击过滤器

对上次左键抬起到下一次左键落下计时，过滤间隔小于25ms的点击事件，防止微动故障导致的双击。

请使用最新版VS2022及`/std=c++latest`选项。

# Double click filter

Measure the time elapsed from the last left mouse button release to the next left mouse button press, filter out click events with intervals smaller than 25ms, to prevent double-clicking caused by microswitch faults.

build with VS2022 and `/std=c++latest` option.