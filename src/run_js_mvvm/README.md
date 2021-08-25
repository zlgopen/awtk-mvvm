# MVVM JS 应用的模拟器

```
Usage: runJsMvvm.exe [start_page=xxx] [system_bar=xxx] [bottom_system_bar=xxx] [res_root=xxx] [lcd_w=800] [lcd_h=480] [custom_widgets_dll_path=xxx] [render_mode=xxx] [run_default=xxx] [is_revert=xxx]
Example:
bin\runJsMvvm.exe start_page=home_page res_root=./res lcd_w=800 lcd_h=480
bin\runJsMvvm.exe start_page=temperature1 "res_root=e:/zlgopen/awtk-mvvm/res" lcd_w=800 lcd_h=480
```

参数说明：
* start_page 可选项，表示应用的启动页，默认值为“home_page”。
* system_bar 可选项，表示应用的顶部系统栏页面，默认为空。
* bottom_system_bar 可选项，表示应用的底部系统栏页面，默认为空。
* res_root 可选项，表示应用的assets资源目录，默认为awtk定义的默认目录。
* lcd_w 可选项，表示LCD的宽度，默认为320。
* lcd_h 可选项，表示LCD的高度，默认为480。
* custom_widgets_dll_path 可选项，表示自定义控件的DLL目录，默认为无。
* render_mode，可选项，表示渲染模式，指OpenGL等，默认为OpenGL。
* run_default，可选项，表示启动时运行默认的界面。
* is_revert，可选项，表示恢复出厂后再启动，即删除已下载的res目录后再启动。
