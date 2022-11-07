#include "mvvm/jerryscript/jsobj.h"
#include "mvvm/jerryscript/object_js_array.h"
#include "fscript_ext/fscript_array.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

TEST(ObjectJsArray, fscript) {
  value_t v;
  const char* code = NULL;
  jsvalue_t jsobj = JS_EMPTY_ARRAY;
  tk_object_t* obj = object_default_create();
  tk_object_t* js_array = object_js_array_create(jsobj, TRUE);
  tk_object_set_prop_object(obj, "js_array", js_array);

  code = "array_push(js_array, 1, 2, 3);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 3);
  ASSERT_EQ(jsobj_get_prop_count(jsobj), 3);

  code = "array_pop(js_array);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 3);
  ASSERT_EQ(jsobj_get_prop_count(jsobj), 2);

  code = "array_shift(js_array);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 1);
  ASSERT_EQ(jsobj_get_prop_count(jsobj), 1);

  code = "array_set(js_array, 0, 9);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_bool(&v), TRUE);

  code = "array_get(js_array, 0);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 9);

  code = "array_insert(js_array, 0, 8);\narray_get(js_array, 0);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 8);
  ASSERT_EQ(jsobj_get_prop_count(jsobj), 2);

  code = "array_remove(js_array, 0);\narray_get(js_array, 0);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 9);
  ASSERT_EQ(jsobj_get_prop_count(jsobj), 1);

  code = "array_get_and_remove(js_array, 0);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 9);
  ASSERT_EQ(jsobj_get_prop_count(jsobj), 0);

  code = "array_push(js_array, 1, 2, 2);\narray_index_of(js_array, 2);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 1);

  code = "array_last_index_of(js_array, 2);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 2);

  code = "array_clear(js_array);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_bool(&v), TRUE);
  ASSERT_EQ(jsobj_get_prop_count(jsobj), 0);

  code = "array_push(js_array, 1, 2, 3);\narray_reverse(js_array);array_join(js_array,\",\");";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "3,2,1");

  code = "array_push(js_array, 5, 6, 4);\narray_sort(js_array);array_join(js_array,\",\");";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "1,2,3,4,5,6");

  code = "array_sort(js_array, false);array_join(js_array,\",\");";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "6,5,4,3,2,1");

  code = "array_min(js_array);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 1);

  code = "array_max(js_array);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 6);

  code = "array_sum(js_array);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 21);

  code = "array_avg(js_array);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_double(&v), 3.5);

  code = "array_is_empty(js_array);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_bool(&v), FALSE);

  code = "array_size(js_array);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 6);

  code = "array_clear(js_array);\narray_is_empty(js_array);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_bool(&v), TRUE);

  code = "array_size(js_array);";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_EQ(value_int32(&v), 0);

  code =
      "array_clear(js_array);\n"
      "array_push(js_array, \"c\", \"B\", \"a\", \"D\");\n"
      "array_sort(js_array);\n"
      "array_join(js_array,\",\");";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "B,D,a,c");

  code =
      "array_sort(js_array, true, true);\n"
      "array_join(js_array,\",\");";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "a,B,c,D");

  code =
      "array_sort(js_array, false, true);\n"
      "array_join(js_array,\",\");";
  ASSERT_EQ(fscript_eval(obj, code, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "D,c,B,a");

  value_reset(&v);
  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(js_array);
}
