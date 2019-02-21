# 本示例展示了MVVM 在窗口之间传递参数的用法。

home是主界面，它调用room_settings对卧室和客厅的进行设置。

## 一、home通过navigator\_request\_t向room\_settings传递参数。

```
  navigator_request_t* req = navigator_request_create("room_settings", home_on_room_result);

  object_set_prop_pointer(OBJECT(req), STR_HOME, h); 
  object_set_prop_str(OBJECT(req), ROOM_SETTINGS_REQ_ARG_ROOM, room_name);
  object_set_prop_float(OBJECT(req), ROOM_SETTINGS_REQ_ARG_TEMP, info->temp);
  object_set_prop_float(OBJECT(req), ROOM_SETTINGS_REQ_ARG_HUMIDITY, info->humidity);

  navigator_to_ex(req);

  object_unref(OBJECT(req));
```

## 二、home通过回调函数home\_on\_room\_result获取结果。

```
static ret_t home_on_room_result(navigator_request_t* req, const value_t* result) {
  object_t* res = value_object(result);
  home_t* h = HOME(object_get_prop_pointer(OBJECT(req), STR_HOME));
  const char* room_name = object_get_prop_str(OBJECT(req), ROOM_SETTINGS_REQ_ARG_ROOM);
  room_info_t* info = tk_str_eq(room_name, STR_BED_ROOM) ? &(h->bed_room) : &(h->living_room);

  info->temp = object_get_prop_float(res, ROOM_SETTINGS_RESULT_TEMP, info->temp);
  info->humidity = object_get_prop_float(res, ROOM_SETTINGS_RESULT_HUMIDITY, info->humidity);

  object_notify_changed(OBJECT(h));

  return RET_OK;
}
```


