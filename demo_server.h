#ifndef __DEMO_SERVER_H__
#define __DEMO_SERVER_H__

#include "demo.h"

typedef struct DemoObj {
    GObject parent;
} DemoObj;

typedef struct DemoObjClass {
    GObjectClass parent;
} DemoObjClass;

static void demo_init (DemoObj *obj) {
}

static void demo_class_init(DemoObjClass * kclass); // 这个方法会被G_DEFINE_TYPE宏生成的代码调用

G_DEFINE_TYPE(DemoObj, demo, G_TYPE_OBJECT)

#define OBJECT_TYPE (demo_get_type()) // demo_get_type函数是由上面的G_DEFINE_TYPE宏生成的


gboolean demo_test_sample_type_in_out(
    DemoObj *obj, 

    gboolean bool_in,
    gdouble double_in,
    gint32 int32_in,
    gint64 int64_in,
    guint8 uint8_in,
    guint32 uint32_in,
    guint64 uint64_in,
    gchar* str_in,

    gboolean* bool_out,
    gdouble* double_out,
    gint32* int32_out,
    gint64* int64_out,
    guint8* uint8_out,
    guint32* uint32_out,
    guint64* uint64_out,
    gchar** str_out,
  
    GError **error);

gboolean demo_test_array_in_out(
    DemoObj *obj, 

    GArray* array_bool_in,
    gchar** array_str_in,

    GArray** array_bool_out,
    gchar*** array_str_out,
  
    GError **error);

gboolean demo_test_value_in_out(
    DemoObj *obj, 

    GValue* value_in,
    GValue* value_out,
  
    GError **error);

gboolean demo_test_struct_in_out(
	DemoObj *obj, 
    
	GValueArray* struct_in, 
	GValueArray** struct_out, 

	GError **error);

gboolean demo_test_struct_array_in_out(
	DemoObj *obj, 
    
	GPtrArray* struct_array_in, 
	GPtrArray** struct_array_out, 

	GError **error);

gboolean demo_test_dict_in_out(
	DemoObj *obj, 
    
	GHashTable* dict_in, 
	GHashTable** dict_out, 

	GError **error);

gboolean demo_test_out_async(
    DemoObj *obj, 
    DBusGMethodInvocation* async_out,
    GError **error);

gboolean demo_test_error(
    DemoObj *obj, 
    GError **error);

gboolean demo_test_send_signal(
    DemoObj *obj, 
    GError **error);

#endif