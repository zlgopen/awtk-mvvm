/**
 * File:   ui_loader_mvvm.c
 * Author: AWTK Develop Team
 * Brief:  ui loader for mvvm
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-03-03 Xu ChaoZe <xuchaoze@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/buffer.h"
#include "tkc/str.h"
#include "tkc/utils.h"
#include "tkc/fscript.h"
#include "ui_loader/ui_binary_writer.h"
#include "ui_loader/ui_loader_xml.h"

#include "ui_loader/ui_builder_default.h"
#include "mvvm/base/binding_rule_parser.h"
#include "mvvm/base/data_binding.h"
#include "mvvm/base/command_binding.h"
#include "mvvm/base/items_binding.h"
#include "mvvm/base/condition_binding.h"
#include "mvvm/awtk/binding_context_awtk.h"
#include "mvvm/awtk/ui_loader_mvvm.h"

static ret_t ui_loader_mvvm_load_a_snippet(ui_loader_mvvm_t* loader, rbuffer_t* rbuffer,
                                           ui_builder_t* b, widget_t* target);

static bool_t rbuffer_find_target_prop(rbuffer_t* rbuffer, const char* target, const char** value) {
  bool_t ret = FALSE;
  const char* key = NULL;
  const char* val = NULL;
  uint32_t old = rbuffer->cursor;
  return_value_if_fail(rbuffer != NULL && target != NULL, FALSE);

  if (rbuffer_skip(rbuffer, sizeof(widget_desc_t)) == RET_OK) {
    if (rbuffer_read_string(rbuffer, &key) == RET_OK) {
      while (*key) {
        if (tk_str_eq(key, target)) {
          ret = TRUE;
          if (value != NULL) {
            rbuffer_read_string(rbuffer, value);
          }
          break;
        }

        break_if_fail(rbuffer_read_string(rbuffer, &val) == RET_OK);
        break_if_fail(rbuffer_read_string(rbuffer, &key) == RET_OK);
      }
    }
  }

  rbuffer_skip(rbuffer, old - rbuffer->cursor);

  return ret;
}

// MVVM_FAST_UI_LOAD模式默认动态渲染规则定义为widget的描述数据(XML)的第1个属性
#ifndef MVVM_FAST_UI_LOAD

static const char* rbuffer_find_dynamic_binding_prop(rbuffer_t* rbuffer, const char** value) {
  const char* type = NULL;
  const char* key = NULL;
  const char* val = NULL;
  uint32_t old = rbuffer->cursor;
  return_value_if_fail(rbuffer != NULL, NULL);

  if (rbuffer_skip(rbuffer, sizeof(widget_desc_t)) == RET_OK) {
    if (rbuffer_read_string(rbuffer, &key) == RET_OK) {
      while (*key) {
        if (tk_str_eq(key, BINDING_RULE_ITEMS)) {
          type = BINDING_RULE_ITEMS;
        } else if (tk_str_eq(key, BINDING_RULE_CONDITION_IF)) {
          type = BINDING_RULE_CONDITION_IF;
        }

        if (type != NULL) {
          if (value != NULL) {
            rbuffer_read_string(rbuffer, value);
          }
          break;
        }

        break_if_fail(rbuffer_read_string(rbuffer, &val) == RET_OK);
        break_if_fail(rbuffer_read_string(rbuffer, &key) == RET_OK);
      }
    }
  }

  rbuffer_skip(rbuffer, old - rbuffer->cursor);

  return type;
}

static bool_t rbuffer_find_target_dynamic_binding_prop(rbuffer_t* rbuffer, const char* target,
                                                       const char** value) {
  return rbuffer_find_target_prop(rbuffer, target, value);
}

#else

static const char* rbuffer_find_dynamic_binding_prop(rbuffer_t* rbuffer, const char** value) {
  const char* type = NULL;
  const char* key = NULL;
  const char* val = NULL;
  uint32_t old = rbuffer->cursor;
  return_value_if_fail(rbuffer != NULL, NULL);

  if (rbuffer_skip(rbuffer, sizeof(widget_desc_t)) == RET_OK) {
    if (rbuffer_read_string(rbuffer, &key) == RET_OK) {
      // widget设置了x/y/w/h，则第一个属性固定为WIDGET_PROP_SELF_LAYOUT，故过滤
      if (*key == 's') {
        rbuffer_read_string(rbuffer, &val);
        rbuffer_read_string(rbuffer, &key);
      }

      if (*key)
        if (tk_str_eq(key, BINDING_RULE_ITEMS)) {
          type = BINDING_RULE_ITEMS;
        } else if (tk_str_eq(key, BINDING_RULE_CONDITION_IF)) {
          type = BINDING_RULE_CONDITION_IF;
        }

      if (type != NULL) {
        if (value != NULL) {
          rbuffer_read_string(rbuffer, value);
        }
      }
    }
  }

  rbuffer_skip(rbuffer, old - rbuffer->cursor);

  return type;
}

static bool_t rbuffer_find_target_dynamic_binding_prop(rbuffer_t* rbuffer, const char* target,
                                                       const char** value) {
  bool_t ret = FALSE;
  const char* key = NULL;
  const char* val = NULL;
  uint32_t old = rbuffer->cursor;
  return_value_if_fail(rbuffer != NULL && target != NULL, FALSE);

  if (rbuffer_skip(rbuffer, sizeof(widget_desc_t)) == RET_OK) {
    if (rbuffer_read_string(rbuffer, &key) == RET_OK) {
      // widget设置了x/y/w/h，则第一个属性固定为WIDGET_PROP_SELF_LAYOUT，故过滤
      if (*key == 's') {
        rbuffer_read_string(rbuffer, &val);
        rbuffer_read_string(rbuffer, &key);
      }

      if (*key) {
        if (tk_str_eq(key, target)) {
          ret = TRUE;
          if (value != NULL) {
            rbuffer_read_string(rbuffer, value);
          }
        }
      }
    }
  }

  rbuffer_skip(rbuffer, old - rbuffer->cursor);

  return ret;
}

#endif /*MVVM_FAST_UI_LOAD*/

static bool_t break_if_equal_target_widget(void* ctx, ui_builder_t* builder) {
  widget_t* target = WIDGET(ctx);
  ENSURE(builder != NULL);

  ui_builder_on_widget_end(builder);
  return (builder->widget != NULL && builder->widget == target) ? TRUE : FALSE;
}

static bool_t break_if_ref_0(void* ctx, ui_builder_t* builder) {
  uint32_t* ref_count = (uint32_t*)(ctx);
  ENSURE(ref_count != NULL);

  (void)builder;

  (*ref_count)--;
  return (*ref_count) == 0 ? TRUE : FALSE;
}

static ret_t rbuffer_skip_widget_end_mark(rbuffer_t* rbuffer, ui_builder_t* builder,
                                          bool_t (*break_func)(void* ctx, ui_builder_t* b),
                                          void* ctx) {
  uint8_t widget_end_mark = 0;

  if (rbuffer_has_more(rbuffer)) {
    return_value_if_fail(rbuffer_peek_uint8(rbuffer, &widget_end_mark) == RET_OK, RET_BAD_PARAMS);

    while (widget_end_mark == 0) {
      rbuffer_read_uint8(rbuffer, &widget_end_mark);

      if (break_func != NULL && break_func(ctx, builder)) {
        break;
      }

      break_if_fail(rbuffer->cursor < rbuffer->capacity);
      break_if_fail(rbuffer_peek_uint8(rbuffer, &widget_end_mark) == RET_OK);
    }
  }

  return RET_OK;
}

static ret_t rbuffer_skip_a_widget(rbuffer_t* rbuffer, ui_builder_t* b) {
  widget_desc_t desc;
  uint32_t ref_count = 0;
  const char* key = NULL;
  const char* val = NULL;

  while ((rbuffer->cursor + sizeof(desc)) <= rbuffer->capacity) {
    ref_count++;

    return_value_if_fail(rbuffer_read_binary(rbuffer, &desc, sizeof(desc)) == RET_OK, RET_FAIL);
    return_value_if_fail(rbuffer_read_string(rbuffer, &key) == RET_OK, RET_FAIL);
    while (*key) {
      return_value_if_fail(rbuffer_read_string(rbuffer, &val) == RET_OK, RET_FAIL);
      return_value_if_fail(rbuffer_read_string(rbuffer, &key) == RET_OK, RET_FAIL);
    }
    return_value_if_fail(
        rbuffer_skip_widget_end_mark(rbuffer, b, break_if_ref_0, &ref_count) == RET_OK, RET_FAIL);

    if (ref_count == 0) {
      break;
    }
  }

  return RET_OK;
}

static ret_t rbuffer_skip_a_condition_widget(rbuffer_t* rbuffer, ui_builder_t* b) {
  ret_t ret = RET_OK;

  do {
    break_if_fail(rbuffer_skip_a_widget(rbuffer, b) == RET_OK);
  } while (rbuffer_find_target_dynamic_binding_prop(rbuffer, BINDING_RULE_CONDITION_ELIF, NULL));

  if (rbuffer_find_target_dynamic_binding_prop(rbuffer, BINDING_RULE_CONDITION_ELSE, NULL)) {
    ret = rbuffer_skip_a_widget(rbuffer, b);
  }

  return ret;
}

static ret_t rbuffer_get_data_range_of_a_widget(rbuffer_t* rbuffer, ui_builder_t* builder,
                                                uint32_t* pos, uint32_t* size) {
  uint32_t old_cursor = rbuffer->cursor;
  return_value_if_fail(rbuffer_skip_a_widget(rbuffer, builder) == RET_OK, RET_BAD_PARAMS);

  *pos = old_cursor;
  *size = rbuffer->cursor - old_cursor;
  rbuffer->cursor = old_cursor;

  return RET_OK;
}

static ret_t rbuffer_get_data_range_of_a_condition_widget(rbuffer_t* rbuffer, ui_builder_t* builder,
                                                          uint32_t* pos, uint32_t* size) {
  uint32_t old_cursor = rbuffer->cursor;
  return_value_if_fail(rbuffer_skip_a_condition_widget(rbuffer, builder) == RET_OK, RET_BAD_PARAMS);

  *pos = old_cursor;
  *size = rbuffer->cursor - old_cursor;
  rbuffer->cursor = old_cursor;

  return RET_OK;
}

static binding_rule_t* ui_loader_mvvm_bind_data(ui_loader_mvvm_t* loader, widget_t* widget,
                                                const char* name, const char* value) {
  binding_rule_t* rule = binding_rule_parse(name, value, widget->vt->inputable);
  data_binding_t* binding = DATA_BINDING(rule);
  return_value_if_fail(binding != NULL, NULL);

  rule->parent = loader->rule;
  rule->widget = widget;

  if (binding_context_bind_data(loader->binding_context, rule) != RET_OK) {
    TK_OBJECT_UNREF(rule);
    return NULL;
  }

  tk_object_set_prop_str(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, "TRUE");

  return rule;
}

static binding_rule_t* ui_loader_mvvm_bind_command(ui_loader_mvvm_t* loader, widget_t* widget,
                                                   const char* name, const char* value) {
  binding_rule_t* rule = binding_rule_parse(name, value, widget->vt->inputable);
  command_binding_t* binding = COMMAND_BINDING(rule);
  return_value_if_fail(binding != NULL, NULL);

  rule->parent = loader->rule;
  rule->widget = widget;

  if (binding_context_bind_command(loader->binding_context, rule) != RET_OK) {
    TK_OBJECT_UNREF(rule);
    return NULL;
  }

  tk_object_set_prop_str(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, "TRUE");

  return rule;
}

static binding_rule_t* ui_loader_mvvm_bind_items(ui_loader_mvvm_t* loader, widget_t* widget,
                                                 const char* name, const char* value,
                                                 uint32_t data_pos, uint32_t data_size) {
  binding_rule_t* rule = binding_rule_parse(name, value, widget->vt->inputable);
  items_binding_t* binding = ITEMS_BINDING(rule);
  return_value_if_fail(binding != NULL, NULL);

  rule->parent = loader->rule;
  rule->widget = widget;
  binding->widget_data_pos = data_pos;
  binding->widget_data_size = data_size;

  if (binding_context_bind_items(loader->binding_context, rule) != RET_OK) {
    TK_OBJECT_UNREF(rule);
    return NULL;
  }

  tk_object_set_prop_str(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, "TRUE");

  return rule;
}

static binding_rule_t* ui_loader_mvvm_bind_condition(ui_loader_mvvm_t* loader, widget_t* widget,
                                                     const char* name, const char* value,
                                                     uint32_t data_pos, uint32_t data_size) {
  binding_rule_t* rule = binding_rule_parse(name, value, widget->vt->inputable);
  condition_binding_t* binding = CONDITION_BINDING(rule);
  return_value_if_fail(binding != NULL, NULL);

  rule->parent = loader->rule;
  rule->widget = widget;
  binding->widget_data_pos = data_pos;
  binding->widget_data_size = data_size;

  if (binding_context_bind_condition(loader->binding_context, rule) != RET_OK) {
    TK_OBJECT_UNREF(rule);
    return NULL;
  }

  tk_object_set_prop_str(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, "TRUE");

  return rule;
}

static ret_t widget_get_custom_prop(widget_t* widget, const char* name, value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (widget->custom_props != NULL) {
    return tk_object_get_prop(widget->custom_props, name, v);
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t widget_set_custom_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (widget->custom_props != NULL) {
    return tk_object_set_prop(widget->custom_props, name, v);
  } else {
    return widget_set_prop(widget, name, v);
  }
}

static ret_t widget_set_custom_prop_uint32(widget_t* widget, const char* name, uint32_t val) {
  value_t v;
  value_set_uint32(&v, val);

  return widget_set_custom_prop(widget, name, &v);
}

static ret_t widget_set_custom_prop_pointer(widget_t* widget, const char* name, void* val) {
  value_t v;
  value_set_pointer(&v, val);

  return widget_set_custom_prop(widget, name, &v);
}

static ret_t widget_init_count_of_widget_before_1st_dynamic_rule(widget_t* widget) {
  value_t v;
  const char* name = WIDGET_PROP_MVVM_COUNT_OF_WIDGET_BEFORE_FIRST_DYNAMIC_RULE;

  if (widget_get_custom_prop(widget, name, &v) != RET_OK) {
    widget_set_custom_prop_uint32(widget, name, widget_count_children(widget));
  }
  return RET_OK;
}

static ret_t visit_clear_binding(void* ctx, const void* data) {
  widget_t* widget = WIDGET(data);
  if (widget->emitter != NULL) {
    widget_off_by_tag(widget, EVENT_TAG);
  }
  return binding_context_clear_bindings_of_widget(ctx, widget);
}

static ret_t widget_destroy_children_and_clear_bindings(widget_t* widget, binding_context_t* ctx) {
  /* 由于控件为异步销毁，无法立即清除绑定规则，故先强制清除，避免无效的绑定被执行 */
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_foreach(iter, visit_clear_binding, ctx);
  WIDGET_FOR_EACH_CHILD_END()

  widget_destroy_children(widget);
  return RET_OK;
}

static ret_t widget_destroy_and_clear_bindings(widget_t* widget, binding_context_t* ctx) {
  /* 由于控件为异步销毁，无法立即清除绑定规则，故先强制清除，避免无效的绑定被执行 */
  widget_foreach(widget, visit_clear_binding, ctx);

  widget_destroy(widget);
  return RET_OK;
}

static ret_t ui_loader_mvvm_on_widget_destroy(void* ctx, event_t* e) {
  binding_context_t* bctx = BINDING_CONTEXT(ctx);
  binding_context_clear_bindings_of_widget(bctx, e->target);
  widget_off_by_ctx(bctx->widget, e->target);

  return RET_REMOVE;
}

static ret_t ui_loader_mvvm_on_widget_binding_context_destroy(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  if (widget->emitter != NULL) {
    widget_off_by_tag(widget, EVENT_TAG);
  }

  (void)e;
  return RET_OK;
}

static ret_t ui_loader_mvvm_build_data_with_widget(ui_loader_mvvm_t* loader,
                                                   darray_t* bind_data_rbuffer_offsets,
                                                   rbuffer_t* rbuffer, widget_t* widget) {
  const char* key = NULL;
  const char* val = NULL;
  uint32_t rbuffer_offset = 0;
  binding_context_t* ctx = loader->binding_context;
  if (ctx != NULL) {
    uint32_t i = 0;
    rbuffer_offset = rbuffer->cursor;
    for (i = 0; i < bind_data_rbuffer_offsets->size; i++) {
      rbuffer_rewind(rbuffer);
      rbuffer_skip(rbuffer, tk_pointer_to_int(darray_get(bind_data_rbuffer_offsets, i)));
      break_if_fail(rbuffer_read_string(rbuffer, &key) == RET_OK);
      break_if_fail(rbuffer_read_string(rbuffer, &val) == RET_OK);
      ui_loader_mvvm_bind_data(loader, widget, key, val);
    }
    rbuffer_rewind(rbuffer);
    rbuffer_skip(rbuffer, rbuffer_offset);
  }
  return RET_OK;
}

static widget_t* ui_loader_mvvm_build_widget(ui_loader_mvvm_t* loader, rbuffer_t* rbuffer,
                                             ui_builder_t* builder, uint32_t* cursor,
                                             const value_t* id) {
  widget_desc_t desc;
  widget_t* widget = NULL;
  const char* vmodel = NULL;
  const char* key = NULL;
  const char* val = NULL;
  uint32_t rbuffer_offset = 0;
  darray_t bind_data_rbuffer_offsets;
  bool_t should_create_binding_context = FALSE;
  binding_context_t* ctx = loader->binding_context;
  navigator_request_t* req = loader->navigator_request;

  if (rbuffer_find_target_prop(rbuffer, WIDGET_PROP_V_MODEL, &val)) {
    vmodel = val;
  }

  return_value_if_fail(rbuffer_read_binary(rbuffer, &desc, sizeof(desc)) == RET_OK, NULL);
  return_value_if_fail(ui_builder_on_widget_start(builder, &desc) == RET_OK, NULL);

  widget = builder->widget;
  should_create_binding_context = (vmodel != NULL) || (ctx == NULL && widget_is_window(widget));

  if (should_create_binding_context) {
    ctx = binding_context_awtk_create(ctx, vmodel, req, widget);
    return_value_if_fail(ctx != NULL, NULL);

    if (loader->binding_context == NULL) {
      widget_set_custom_prop_pointer(widget, WIDGET_PROP_MVVM_ASSETS_INFO, (void*)(loader->ui));
    }
    loader->binding_context = ctx;
  }

  if (cursor != NULL) {
    widget_set_custom_prop_uint32(widget, WIDGET_PROP_MVVM_DATA_CURSOR, *cursor);
  }

  rbuffer_offset = rbuffer->cursor;
  darray_init(&bind_data_rbuffer_offsets, 10, NULL, NULL);
  goto_error_if_fail(rbuffer_read_string(rbuffer, &key) == RET_OK);
  while (*key) {
    goto_error_if_fail(rbuffer_read_string(rbuffer, &val) == RET_OK);
    if (tk_str_start_with(key, BINDING_RULE_DATA_PREFIX)) {
      /* 记录绑定数据的信息，统一在最后绑定数据 */
      darray_push(&bind_data_rbuffer_offsets, tk_pointer_from_int(rbuffer_offset));
    } else if (tk_str_start_with(key, BINDING_RULE_COMMAND_PREFIX)) {
      if (ctx != NULL) ui_loader_mvvm_bind_command(loader, widget, key, val);
    } else if (tk_str_eq(key, BINDING_RULE_ITEMS)) {
    } else if (tk_str_eq(key, BINDING_RULE_CONDITION_IF)) {
    } else if (tk_str_eq(key, BINDING_RULE_CONDITION_ELIF)) {
    } else if (tk_str_eq(key, BINDING_RULE_CONDITION_ELSE)) {
    } else if (tk_str_eq(key, WIDGET_PROP_V_MODEL)) {
    } else {
      ui_builder_on_widget_prop(builder, key, val);
    }
    rbuffer_offset = rbuffer->cursor;
    goto_error_if_fail(rbuffer_read_string(rbuffer, &key) == RET_OK);
  }

  if (id != NULL) {
    widget_set_custom_prop(widget, WIDGET_PROP_MVVM_WIDGET_ID, id);
  }

  goto_error_if_fail(ui_builder_on_widget_prop_end(builder) == RET_OK);

  /* 统一绑定数据，用于确保先触发修改属性再触发相关个注册事件 */
  ui_loader_mvvm_build_data_with_widget(loader, &bind_data_rbuffer_offsets, rbuffer, widget);
  darray_deinit(&bind_data_rbuffer_offsets);

  // 跳过控件的结束标记，直到当前控件等于widget的父控件
  goto_error_if_fail(rbuffer_skip_widget_end_mark(rbuffer, builder, break_if_equal_target_widget,
                                                  widget->parent) == RET_OK);

  // 如果当前控件不是widget的父控件，则表示含有子控件，因此继续构建子控件
  if (builder->widget != widget->parent) {
    goto_error_if_fail(ui_loader_mvvm_load_a_snippet(loader, rbuffer, builder, widget->parent) ==
                       RET_OK);
  }

  if (should_create_binding_context) {
    binding_context_set_bound(ctx, TRUE);
    loader->binding_context = ctx->parent;
  }

  widget_on_with_tag(widget, EVT_DESTROY, ui_loader_mvvm_on_widget_destroy, ctx, EVENT_TAG);
  if (ctx->widget != widget) {
    widget_on_with_tag(WIDGET(ctx->widget), EVT_DESTROY, ui_loader_mvvm_on_widget_binding_context_destroy, widget, EVENT_TAG);
  }

  return widget;

error:
  darray_deinit(&bind_data_rbuffer_offsets);
  widget_destroy(widget);
  return NULL;
}

static ret_t ui_loader_mvvm_build_condition_widget(ui_loader_mvvm_t* loader, rbuffer_t* rbuffer,
                                                   ui_builder_t* builder, binding_rule_t* rule) {
  bool_t is_ok = FALSE;
  const char* expr = NULL;
  uint32_t offset = 0;
  widget_t* parent;
  binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);
  condition_binding_t* binding = CONDITION_BINDING(rule);
  return_value_if_fail(binding != NULL, RET_BAD_PARAMS);

  offset = binding->widget_data_pos - rbuffer->cursor;
  return_value_if_fail(rbuffer_skip(rbuffer, offset) == RET_OK, RET_BAD_PARAMS);
  return_value_if_fail(
      rbuffer_find_target_dynamic_binding_prop(rbuffer, BINDING_RULE_CONDITION_IF, &expr),
      RET_BAD_PARAMS);

  parent = WIDGET(BINDING_RULE_WIDGET(rule));
  widget_init_count_of_widget_before_1st_dynamic_rule(parent);

  while (TRUE) {
    if (!is_ok) {
      value_t v;
      str_t str;
      uint32_t len = 0;
      const char* l_str = *expr == '{' ? (expr + 1) : expr;
      const char* r_str = tk_strrstr(l_str, "}");
      len = r_str != NULL ? (r_str - l_str) : tk_strlen(l_str);
      str_init(&str, 0);
      str_set_with_len(&str, l_str, len);
      value_set_bool(&v, FALSE);
      if (fscript_eval(TK_OBJECT(rule), str.str, &v) == RET_OK && value_bool(&v)) {
        is_ok = TRUE;
      }
      value_reset(&v);
      str_reset(&str);
    }

    if (!is_ok) {
      // 条件为false，则跳过
      return_value_if_fail(rbuffer_skip_a_widget(rbuffer, builder) == RET_OK, RET_FAIL);
    } else {
      // 条件为true，且与上一次的不同，则新建对应的控件
      if (!tk_str_eq(expr, binding->current_expr)) {
        uint32_t index = binding_context_calc_widget_index_of_rule(ctx, rule);
        widget_t* widget = NULL;

        // 销毁旧的控件
        if (binding->current_expr != NULL) {
          widget = widget_get_child(parent, index);
          ENSURE(widget != NULL);
          widget_destroy_and_clear_bindings(widget, ctx);
        }

        widget = ui_loader_mvvm_build_widget(loader, rbuffer, builder, NULL, NULL);
        return_value_if_fail(widget != NULL, RET_FAIL);

        if (index != widget_count_children(parent) - 1) {
          widget_restack(widget, index);
        }

        binding->current_expr = expr;
      }

      offset = binding->widget_data_pos + binding->widget_data_size;
      return rbuffer_skip(rbuffer, offset - rbuffer->cursor);
    }

    if (!rbuffer_find_target_dynamic_binding_prop(rbuffer, BINDING_RULE_CONDITION_ELIF, &expr)) {
      if (rbuffer_find_target_dynamic_binding_prop(rbuffer, BINDING_RULE_CONDITION_ELSE, &expr)) {
        is_ok = TRUE;
      } else {
        break;
      }
    }
  }

  // 未找到条件为true的控件，销毁旧的控件
  if (binding->current_expr != NULL) {
    uint32_t index = binding_context_calc_widget_index_of_rule(ctx, rule);
    widget_t* widget = widget_get_child(parent, index);
    ENSURE(widget != NULL);
    widget_destroy_and_clear_bindings(widget, ctx);
  }

  binding->current_expr = NULL;
  return RET_OK;
}

widget_t* widget_lookup_by_id(widget_t* widget, const value_t* id, int32_t begin_index,
                              int32_t end_index, uint32_t* ret_index) {
  int32_t delta = 0;
  int32_t nr = 0;
  widget_t** children = NULL;
  widget_t* child;
  value_t v;
  return_value_if_fail(widget != NULL && id != NULL, NULL);

  nr = widget_count_children(widget);
  return_value_if_fail(begin_index < nr && end_index < nr, NULL);

  delta = begin_index <= end_index ? 1 : -1;
  children = (widget_t**)(widget->children->elms);

  while ((delta * (end_index - begin_index)) >= 0) {
    child = children[begin_index];
    if (widget_get_custom_prop(child, WIDGET_PROP_MVVM_WIDGET_ID, &v) == RET_OK) {
      if (value_equal(&v, id)) {
        if (ret_index != NULL) {
          *ret_index = begin_index;
        }
        return child;
      }
    }
    begin_index += delta;
  }

  return NULL;
}

static ret_t ui_loader_mvvm_get_widget_id(binding_context_t* ctx, binding_rule_t* rule,
                                          tk_object_t* obj, const char* id_name, value_t* v) {
  bool_t is_cursor = FALSE;
  const char* id = binding_context_resolve_path_by_rule(ctx, rule, id_name, &is_cursor);

  if (is_cursor) {
    value_set_str(v, id);
    return RET_OK;
  } else {
    return tk_object_get_prop(obj, id, v);
  }
}

static ret_t ui_loader_mvvm_build_items_widget(ui_loader_mvvm_t* loader, rbuffer_t* rbuffer,
                                               ui_builder_t* builder, binding_rule_t* rule) {
  uint32_t offset = 0;
  widget_t* parent;
  binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);
  items_binding_t* binding = ITEMS_BINDING(rule);
  view_model_t* view_model = BINDING_RULE_VIEW_MODEL(rule);
  return_value_if_fail(binding != NULL && view_model != NULL, RET_BAD_PARAMS);

  offset = binding->widget_data_pos - rbuffer->cursor;
  return_value_if_fail(rbuffer_skip(rbuffer, offset) == RET_OK, RET_BAD_PARAMS);

  parent = WIDGET(BINDING_RULE_WIDGET(rule));
  widget_init_count_of_widget_before_1st_dynamic_rule(parent);

  binding->bound = FALSE;

  if (binding->fixed_widget_count >= 0) {
    int32_t first_index = binding_context_calc_widget_index_of_rule(ctx, rule);
    int32_t nr = widget_count_children(parent);

    if (nr < first_index + binding->fixed_widget_count) {
      uint32_t i;
      widget_t* widget;

      for (i = 0; i < first_index + binding->fixed_widget_count - nr; i++) {
        binding->cursor = nr + i;

        rbuffer->cursor = binding->widget_data_pos;
        widget = ui_loader_mvvm_build_widget(loader, rbuffer, builder, &i, NULL);
        return_value_if_fail(widget != NULL, RET_FAIL);
      }
    }
  } else {
    value_t v;
    tk_object_t* obj = NULL;
    const char* items_name = binding->items_name;
    const char* items_length_name = TK_OBJECT_PROP_SIZE;

    if (binding_context_get_prop_by_rule(ctx, rule, items_name, &v) == RET_OK) {
      obj = value_object(&v);
    }

    if (obj == TK_OBJECT(view_model) && tk_object_is_collection(obj)) {
      items_length_name = VIEW_MODEL_PROP_ITEMS;
    }

    if (obj == NULL) {
      widget_t* widget = NULL;
      uint32_t old_count = binding->items_count;

      if (old_count == widget_count_children(parent)) {
        widget_destroy_children_and_clear_bindings(parent, ctx);
      } else {
        uint32_t i;
        uint32_t first_index = binding_context_calc_widget_index_of_rule(ctx, rule);

        for (i = 0; i < old_count; i++) {
          widget = widget_get_child(parent, first_index + old_count - i - 1);
          ENSURE(widget != NULL);
          widget_destroy_and_clear_bindings(widget, ctx);
        }
      }

      binding->items_count = 0;
    } else {
      uint32_t i = 0;
      widget_t* widget = NULL;
      uint32_t first_index = binding_context_calc_widget_index_of_rule(ctx, rule);
      uint32_t new_count = tk_object_get_prop_uint32(obj, items_length_name, 0);
      uint32_t old_count = binding->items_count;
      const char* id_name = binding->id_name;
      value_t* pid = NULL;
      value_t id;

      if (new_count == 0 && old_count == widget_count_children(parent)) {
        widget_destroy_children_and_clear_bindings(parent, ctx);
      } else if (id_name == NULL || old_count == 0) {
        for (i = new_count; i < old_count; i++) {
          widget = widget_get_child(parent, first_index + new_count + old_count - i - 1);
          ENSURE(widget != NULL);
          widget_destroy_and_clear_bindings(widget, ctx);
        }

        for (i = old_count; i < new_count; i++) {
          pid = NULL;
          binding->cursor = i;
          if (id_name != NULL) {
            if (ui_loader_mvvm_get_widget_id(ctx, rule, TK_OBJECT(view_model), id_name, &id) ==
                RET_OK) {
              pid = &id;
            }
          }

          rbuffer->cursor = binding->widget_data_pos;
          widget = ui_loader_mvvm_build_widget(loader, rbuffer, builder, &i, pid);
          return_value_if_fail(widget != NULL, RET_FAIL);

          if (first_index + i != widget_count_children(widget->parent) - 1) {
            widget_restack(widget, first_index + i);
          }
        }
      } else {
        uint32_t old_begin = first_index;
        uint32_t old_end = first_index + old_count - 1;
        uint32_t new_begin = 0;
        uint32_t new_end = new_count - 1;

        while (new_begin < new_end && old_begin < old_end) {
          pid = NULL;
          binding->cursor = new_begin;
          if (ui_loader_mvvm_get_widget_id(ctx, rule, TK_OBJECT(view_model), id_name, &id) ==
              RET_OK) {
            pid = &id;
            widget = widget_lookup_by_id(parent, pid, old_begin, old_end, &i);
          }

          if (widget != NULL) {
            if (i != old_begin) {
              widget_restack(widget, old_begin);
            }

            widget_set_custom_prop_uint32(widget, WIDGET_PROP_MVVM_DATA_CURSOR, new_begin);
            old_begin++;
          } else {
            rbuffer->cursor = binding->widget_data_pos;
            widget = ui_loader_mvvm_build_widget(loader, rbuffer, builder, &new_begin, pid);
            return_value_if_fail(widget != NULL, RET_FAIL);

            widget_restack(widget, old_begin);
            old_begin++;
            old_end++;
          }

          new_begin++;

          pid = NULL;
          binding->cursor = new_end;
          if (ui_loader_mvvm_get_widget_id(ctx, rule, TK_OBJECT(view_model), id_name, &id) ==
              RET_OK) {
            pid = &id;
            widget = widget_lookup_by_id(parent, pid, old_end, old_begin, &i);
          }

          if (widget != NULL) {
            if (i != old_end) {
              widget_restack(widget, old_end);
            }

            widget_set_custom_prop_uint32(widget, WIDGET_PROP_MVVM_DATA_CURSOR, new_end);
            old_end--;
          } else {
            rbuffer->cursor = binding->widget_data_pos;
            widget = ui_loader_mvvm_build_widget(loader, rbuffer, builder, &new_end, pid);
            return_value_if_fail(widget != NULL, RET_FAIL);

            widget_restack(widget, old_end + 1);
          }

          new_end--;
        }

        for (i = old_end; i >= old_begin; i--) {
          widget = widget_get_child(parent, i);
          ENSURE(widget != NULL);
          widget_destroy_and_clear_bindings(widget, ctx);
          if (i == 0) {
            break;
          }
        }

        for (i = new_begin; i <= new_end; i++) {
          pid = NULL;
          binding->cursor = i;
          if (ui_loader_mvvm_get_widget_id(ctx, rule, TK_OBJECT(view_model), id_name, &id) ==
              RET_OK) {
            pid = &id;
          }

          rbuffer->cursor = binding->widget_data_pos;
          widget = ui_loader_mvvm_build_widget(loader, rbuffer, builder, &i, pid);
          return_value_if_fail(widget != NULL, RET_FAIL);

          widget_restack(widget, first_index + i);
        }
      }

      binding->items_count = new_count;
    }
  }

  binding->bound = TRUE;
  rbuffer->cursor = binding->widget_data_pos + binding->widget_data_size;

  return RET_OK;
}

static uint32_t ui_loader_mvvm_count_bindings(slist_t* list, widget_t* widget) {
  darray_t* node = NULL;
  return_value_if_fail(list != NULL && widget != NULL, 0);

  if (list->first != NULL) {
    node = slist_find(list, widget);
    if (node != NULL) {
      return node->size;
    }
  }

  return 0;
}

static ret_t ui_loader_mvvm_load_a_snippet(ui_loader_mvvm_t* loader, rbuffer_t* rbuffer,
                                           ui_builder_t* builder, widget_t* end_widget) {
  const char* key = NULL;
  const char* val = NULL;
  binding_rule_t* rule;
  widget_t* widget;
  binding_context_t* ctx;

  while ((rbuffer->cursor + sizeof(widget_desc_t)) <= rbuffer->capacity) {
    widget = builder->widget;
    ctx = loader->binding_context;

    if (ctx != NULL) {
      key = rbuffer_find_dynamic_binding_prop(rbuffer, &val);

      if (key == NULL) {
        if (widget->custom_props != NULL && widget_count_children(widget) == 0 &&
            ui_loader_mvvm_count_bindings(&(ctx->dynamic_bindings), widget) == 0 &&
            tk_object_get_prop_bool(widget->custom_props, WIDGET_PROP_V_FOR_ITEMS, FALSE)) {
          key = BINDING_RULE_ITEMS;
          val = "";
        }
      }
    }

    if (key == NULL) {
      if (ctx != NULL && widget != NULL) {
        rule = loader->rule;
        if (rule != NULL && rule->widget == widget) {
          if (binding_rule_is_items_binding(rule)) {
            items_binding_t* binding = ITEMS_BINDING(rule);
            binding->static_widget_before_next_dynamic_binding++;
          } else if (binding_rule_is_condition_binding(rule)) {
            condition_binding_t* binding = CONDITION_BINDING(rule);
            binding->static_widget_before_next_dynamic_binding++;
          }
        }
      }

      return_value_if_fail(
          ui_loader_mvvm_build_widget(loader, rbuffer, builder, NULL, NULL) != NULL, RET_FAIL);
    } else if (tk_str_eq(key, BINDING_RULE_ITEMS)) {
      uint32_t pos, size;
      return_value_if_fail(
          rbuffer_get_data_range_of_a_widget(rbuffer, builder, &pos, &size) == RET_OK, RET_FAIL);

      rule = ui_loader_mvvm_bind_items(loader, widget, key, val, pos, size);
      return_value_if_fail(rule != NULL, RET_FAIL);

      loader->rule = rule;
      return_value_if_fail(
          ui_loader_mvvm_build_items_widget(loader, rbuffer, builder, rule) == RET_OK, RET_FAIL);
      loader->rule = rule->parent;
    } else if (tk_str_eq(key, BINDING_RULE_CONDITION_IF)) {
      uint32_t pos, size;
      return_value_if_fail(
          rbuffer_get_data_range_of_a_condition_widget(rbuffer, builder, &pos, &size) == RET_OK,
          RET_FAIL);

      rule = ui_loader_mvvm_bind_condition(loader, widget, key, val, pos, size);
      return_value_if_fail(rule != NULL, RET_FAIL);
      loader->rule = rule;
      return_value_if_fail(
          ui_loader_mvvm_build_condition_widget(loader, rbuffer, builder, rule) == RET_OK,
          RET_FAIL);
      loader->rule = rule->parent;
    } else {
      if (rbuffer_skip_a_widget(rbuffer, builder) != RET_OK) {
        return RET_FAIL;
      }
    }

    rbuffer_skip_widget_end_mark(rbuffer, builder, break_if_equal_target_widget, end_widget);

    if (end_widget != NULL && builder->widget == end_widget) {
      break;
    }
  }

  return RET_OK;
}

static ret_t ui_loader_mvvm_load_bin(ui_loader_t* l, const uint8_t* data, uint32_t size,
                                     ui_builder_t* b) {
  ui_loader_mvvm_t* loader = UI_LOADER_MVVM(l);
  rbuffer_t rbuffer;
  uint32_t magic = 0;
  binding_rule_t* rule = NULL;

  return_value_if_fail(loader != NULL && data != NULL && b != NULL, RET_BAD_PARAMS);
  return_value_if_fail(rbuffer_init(&rbuffer, data, size) != NULL, RET_BAD_PARAMS);
  return_value_if_fail(rbuffer_read_uint32(&rbuffer, &magic) == RET_OK, RET_BAD_PARAMS);
  return_value_if_fail(magic == UI_DATA_MAGIC, RET_BAD_PARAMS);

  rule = loader->rule;
  if (rule == NULL) {
    ui_builder_on_start(b);
    ui_loader_mvvm_load_a_snippet(loader, &rbuffer, b, b->widget);
    ui_builder_on_end(b);
  } else {
    widget_t* widget = WIDGET(BINDING_RULE_WIDGET(rule));

    if (binding_rule_is_items_binding(rule)) {
      b->widget = widget;
      ui_loader_mvvm_build_items_widget(loader, &rbuffer, b, rule);
      if (ITEMS_BINDING(rule)->fixed_widget_count <= 0) {
        widget_layout(widget);
      }
    } else if (binding_rule_is_condition_binding(rule)) {
      b->widget = widget;
      ui_loader_mvvm_build_condition_widget(loader, &rbuffer, b, rule);
      widget_layout(widget);
    }
  }

  return RET_OK;
}

static ret_t ui_loader_mvvm_load_xml(ui_loader_t* l, const uint8_t* data, uint32_t size,
                                     ui_builder_t* b) {
  wbuffer_t wbuffer;
  ret_t ret = RET_OK;
  ui_binary_writer_t ui_binary_writer;
  ui_loader_t* loader = xml_ui_loader();
  ui_builder_t* builder =
      ui_binary_writer_init(&ui_binary_writer, wbuffer_init_extendable(&wbuffer));

  wbuffer_extend_capacity(&wbuffer, size);
  ui_loader_load(loader, (const uint8_t*)data, size, builder);
  ret = ui_loader_mvvm_load_bin(l, wbuffer.data, wbuffer.cursor, b);
  wbuffer_deinit(&wbuffer);

  return ret;
}

static ret_t ui_loader_mvvm_load(ui_loader_t* l, const uint8_t* data, uint32_t size,
                                 ui_builder_t* b) {
  if (*data == '<') {
    return ui_loader_mvvm_load_xml(l, data, size, b);
  } else {
    return ui_loader_mvvm_load_bin(l, data, size, b);
  }
}

static ui_loader_mvvm_t s_ui_loader_mvvm;

ui_loader_t* ui_loader_mvvm() {
  memset(&s_ui_loader_mvvm, 0x00, sizeof(ui_loader_mvvm_t));
  s_ui_loader_mvvm.base.load = ui_loader_mvvm_load;
  return (ui_loader_t*)&s_ui_loader_mvvm;
}

ui_loader_mvvm_t* ui_loader_mvvm_cast(ui_loader_t* loader) {
  return_value_if_fail(loader != NULL && loader->load == ui_loader_mvvm_load, NULL);
  return (ui_loader_mvvm_t*)loader;
}

widget_t* ui_loader_mvvm_load_widget(navigator_request_t* req) {
  return ui_loader_mvvm_load_widget_with_parent(req, NULL);
}

widget_t* ui_loader_mvvm_load_widget_with_parent(navigator_request_t* req, widget_t* parent) {
  const asset_info_t* ui = NULL;
  const char* target = NULL;
  widget_t* root = NULL;
  char applet_name[TK_NAME_LEN + 1] = {0};
  assets_manager_t* am = assets_manager();
  return_value_if_fail(req != NULL, NULL);

  target = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET);
  return_value_if_fail(target != NULL, NULL);

  if (strncmp(target, STR_SCHEMA_FILE, strlen(STR_SCHEMA_FILE)) != 0 &&
      assets_managers_is_applet_assets_supported()) {
    const char* p = strchr(target, '.');
    if (p != NULL) {
      tk_strncpy_s(applet_name, sizeof(applet_name) - 1, target, p - target);
      am = assets_managers_ref(applet_name);
      target = p + 1;
    }
  }

  ui = assets_manager_ref(am, ASSET_TYPE_UI, target);
  if (ui != NULL && ui->data != NULL && ui->size > 0) {
    ui_loader_t* loader = ui_loader_mvvm();
    ui_builder_t* builder = ui_builder_default_create(target);

    if (builder != NULL) {
      builder->widget = parent;
      UI_LOADER_MVVM(loader)->navigator_request = req;
      UI_LOADER_MVVM(loader)->ui = ui;
      ui_loader_mvvm_load(loader, ui->data, ui->size, builder);
      root = builder->root;
      ui_builder_destroy(builder);
    }

    if (root == NULL) {
      assets_manager_unref(am, ui);
    }
  }

  if (applet_name[0]) {
    assets_managers_unref(am);
  }

  return root;
}

static const asset_info_t* ui_loader_mvvm_get_ui_from_rule(binding_rule_t* rule) {
  binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);
  widget_t* widget = NULL;
  asset_info_t* ui = NULL;
  value_t v;

  while (ctx != NULL && ctx->widget != NULL) {
    widget = WIDGET(ctx->widget);
    if (widget_get_custom_prop(widget, WIDGET_PROP_MVVM_ASSETS_INFO, &v) == RET_OK) {
      ui = (asset_info_t*)value_pointer(&v);
      if (ui != NULL) {
        break;
      }
    }
    ctx = ctx->parent;
  }

  return ui;
}

ret_t ui_loader_mvvm_reload_widget(binding_rule_t* rule) {
  ret_t ret = RET_OK;
  const asset_info_t* ui = NULL;
  ui_loader_t* loader = ui_loader_mvvm();
  ui_builder_t* builder = ui_builder_default_create(NULL);
  binding_context_t* ctx;
  return_value_if_fail(rule != NULL && builder != NULL, RET_BAD_PARAMS);

  ctx = BINDING_RULE_CONTEXT(rule);
  return_value_if_fail(ctx != NULL && ctx->navigator_request != NULL, RET_BAD_PARAMS);

  ui = ui_loader_mvvm_get_ui_from_rule(rule);
  return_value_if_fail(ui != NULL && ui->data != NULL && ui->size > 0, RET_BAD_PARAMS);

  UI_LOADER_MVVM(loader)->ui = ui;
  UI_LOADER_MVVM(loader)->rule = rule;
  UI_LOADER_MVVM(loader)->binding_context = ctx;
  UI_LOADER_MVVM(loader)->navigator_request = ctx->navigator_request;

  ret = ui_loader_mvvm_load(loader, ui->data, ui->size, builder);
  ui_builder_destroy(builder);
  return ret;
}
