#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "demo_server.h"
#include "demo_server_glue.h"

enum {
	SIG_TEST_SIGNAL,
	SIG_LAST_SIGNAL
};

static guint signals[SIG_LAST_SIGNAL] = { 0 };

static void demo_class_init(DemoObjClass * kclass) {
	printf("demo_class_init\n");

	signals[SIG_TEST_SIGNAL] = g_signal_new (
            "test_signal",
            G_OBJECT_CLASS_TYPE(kclass),
            G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__STRING,
            G_TYPE_NONE,
            1,
            dbus_g_type_get_map("GHashTable", G_TYPE_STRING, G_TYPE_STRING));
}

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

	GError **error) {
    *bool_out = !bool_in,
    *double_out = double_in + 1,
    *int32_out = int32_in + 1,
    *int64_out = int64_in + 1,
    *uint8_out = uint8_in + 1,
    *uint32_out = uint32_in + 1,
    *uint64_out = uint64_in + 1,
	printf("demo_test_sample_type_in_out:\n");
	printf("\tbool_in = %s\n" , bool_in ? "True" : "False");
	printf("\tdouble_in = %f\n" , double_in);
	printf("\tint32_in = %d\n" , (int)int32_in);
	printf("\tint64_in = %d\n" , (int)int64_in);
	printf("\tuint8_in = %d\n" , (int)uint8_in);
	printf("\tuint32_in = %d\n" ,(int)uint32_in);
	printf("\tuint64_in = %d\n" , (int)uint64_in);
	printf("\tstr_in = %s, %d, %d\n" , str_in, (int)int64_in, (int)uint64_in);
	printf("\n");
    gchar* concat = g_strdup("out : ");
    *str_out = g_strconcat(concat, str_in, NULL);
	return TRUE;
}

gboolean demo_test_value_in_out(
    DemoObj *obj, 

    GValue* value_in,
    GValue* value_out,
  
    GError **error) {
		printf("demo_test_value_in_out:\n");

		switch(G_VALUE_TYPE(value_in)) {
			case G_TYPE_STRING: 
				printf("\tvalue_in : %s\n", g_value_get_string(value_in));
				g_value_init(value_out, G_TYPE_STRING);
				g_value_set_string(value_out, g_value_get_string(value_in));
				break;
			
			case G_TYPE_INT: 
				printf("\tvalue_in : %d\n", g_value_get_int(value_in));
				g_value_init(value_out, G_TYPE_INT);
				g_value_set_int(value_out, g_value_get_int(value_in));
				break;

			case G_TYPE_DOUBLE: 
				printf("\tvalue_in : %lf\n", g_value_get_double(value_in));
				g_value_init(value_out, G_TYPE_DOUBLE);
				g_value_set_double(value_out, g_value_get_double(value_in));
				break;
			
			case G_TYPE_BOOLEAN: 
				printf("\tvalue_in : %s\n", g_value_get_boolean(value_in) ? "True" : "False");
				g_value_init(value_out, G_TYPE_BOOLEAN);
				g_value_set_boolean(value_out, g_value_get_boolean(value_in));
				break;

			default:
				printf("\tnot support type: %x\n", G_VALUE_TYPE(value_in));
				g_value_init(value_out, G_TYPE_STRING);
				g_value_set_static_string(value_out, "type not support!!");
				break;
		}
		printf("\n");
		return TRUE;
}


int get_str_array_size(gchar** array) {
	int size = 0;
	while(*array != NULL) {
		size++;
		array++;
	}

	return size;
}

gboolean demo_test_array_in_out(
    DemoObj *obj, 

    GArray* array_bool_in,
    gchar** array_str_in,

    GArray** array_bool_out,
    gchar*** array_str_out,
  
    GError **error) {
		printf("demo_test_array_in_out:\n");
		*array_bool_out = g_array_new(FALSE, FALSE, sizeof(gboolean));
		for(int i = 0 ; i<array_bool_in->len ; i++) {
			gboolean b = g_array_index(array_bool_in, gboolean, i);
			g_array_append_val(*array_bool_out, b);
			printf("\tarray_bool_in %d : %s\n", i, b ? "True" : "False");
		}


		int index = 0;
		*array_str_out = malloc((get_str_array_size(array_str_in)+1) * sizeof(gchar*));
		while(*array_str_in != NULL) {
			printf("\tarray_str_in %d : %s\n", index, *array_str_in);
			(*array_str_out)[index++] = g_strconcat(g_strdup("out "), *array_str_in, NULL);
			array_str_in++;
		}
		(*array_str_out)[index] = NULL;
		printf("\n");
}

gboolean demo_test_struct_in_out(
	DemoObj *obj, 
	GValueArray* struct_in, 
	GValueArray** struct_out, 
	GError **error) {
		printf("demo_test_struct_in_out:\n");
		*struct_out = g_value_array_new(struct_in->n_values);

		for(int i = 0 ; i <struct_in->n_values ; i++) {
			GValue* value_in = g_value_array_get_nth(struct_in, i);

			// g_value_array_append尾部插入
			// g_value_array_prepend头部插入
			g_value_array_append(*struct_out, NULL); 

			GValue* value_out;

			switch(G_VALUE_TYPE(value_in)) {
				case G_TYPE_STRING: 
					printf("\tstruct_in %d : %s\n", i, g_value_get_string(value_in));
					value_out = g_value_array_get_nth(*struct_out, i);
					g_value_init(value_out, G_TYPE_STRING);
					g_value_set_string(value_out, g_value_get_string(value_in));
					break;
				
				case G_TYPE_INT: 
					printf("\tstruct_in %d : %d\n", i, g_value_get_int(value_in));
					value_out = g_value_array_get_nth(*struct_out, i);
					g_value_init(value_out, G_TYPE_INT);
					g_value_set_int(value_out, g_value_get_int(value_in));
					break;

				case G_TYPE_DOUBLE: 
					printf("struct_in %d : %f\n", i, g_value_get_double(value_in));
					value_out = g_value_array_get_nth(*struct_out, i);
					g_value_init(value_out, G_TYPE_DOUBLE);
					g_value_set_double(value_out, g_value_get_double(value_in));
					break;
				
				case G_TYPE_BOOLEAN: 
					printf("\tstruct_in %d : %s\n", i, g_value_get_boolean(value_in) ? "True" : "False");
					value_out = g_value_array_get_nth(*struct_out, i);
					g_value_init(value_out, G_TYPE_BOOLEAN);
					g_value_set_boolean(value_out, g_value_get_boolean(value_in));
					break;

				default:
					printf("\tnot support type: %x\n", G_VALUE_TYPE(value_in));
					return FALSE;
			}
		}
		printf("\n");

   return TRUE;
}

gboolean demo_test_struct_array_in_out(
	DemoObj *obj, 
    
	GPtrArray* struct_array_in, 
	GPtrArray** struct_array_out, 

	GError **error) {
		printf("demo_test_struct_array_in_out:\n");
		*struct_array_out = g_ptr_array_new();
		for(int i = 0 ; i<struct_array_in->len ; i++) {
			GValueArray* struct_in = g_ptr_array_index(struct_array_in, i);

			GValueArray* struct_out = g_value_array_new(struct_in->n_values);
			for(int j = 0 ; j <struct_in->n_values ; j++) {
				GValue* value_in = g_value_array_get_nth(struct_in, j);

				// g_value_array_append尾部插入
				// g_value_array_prepend头部插入
				g_value_array_append(struct_out, NULL); 

				GValue* value_out;

				switch(G_VALUE_TYPE(value_in)) {
					case G_TYPE_STRING: 
						value_out = g_value_array_get_nth(struct_out, j);
						g_value_init(value_out, G_TYPE_STRING);
						g_value_set_string(value_out, g_value_get_string(value_in));
						printf("\tstruct_array_in %d, %d : %s\n", i, j, g_value_get_string(value_in));
						break;
					
					case G_TYPE_INT: 
						value_out = g_value_array_get_nth(struct_out, j);
						g_value_init(value_out, G_TYPE_INT);
						g_value_set_int(value_out, g_value_get_int(value_in));
						printf("\tstruct_array_in %d, %d : %d\n", i, j, g_value_get_int(value_in));
						break;

					case G_TYPE_DOUBLE: 
						value_out = g_value_array_get_nth(struct_out, j);
						g_value_init(value_out, G_TYPE_DOUBLE);
						g_value_set_double(value_out, g_value_get_double(value_in));
						printf("\tstruct_array_in %d, %d : %f\n", i, j, g_value_get_double(value_in));
						break;
					
					case G_TYPE_BOOLEAN: 
						value_out = g_value_array_get_nth(struct_out, j);
						g_value_init(value_out, G_TYPE_BOOLEAN);
						g_value_set_boolean(value_out, g_value_get_boolean(value_in));
						printf("\tstruct_array_in %d, %d : %s\n", i, j, g_value_get_boolean(value_in) ? "True" : "False");
						break;

					default:
						printf("\tnot support type: %x\n", G_VALUE_TYPE(value_in));
						return FALSE;
				}
			}

			g_ptr_array_add(*struct_array_out, struct_out);
		}

		printf("\n");

	return TRUE;
}

gboolean demo_test_dict_in_out(
	DemoObj *obj, 

	GHashTable* dict_in, 
	GHashTable** dict_out, 

	GError **error) {
		printf("demo_test_dict_in_out:\n");
		*dict_out = g_hash_table_new(g_str_hash, g_str_equal);

		GHashTableIter iter;
		g_hash_table_iter_init(&iter, dict_in);
		gpointer key, value;
		while(g_hash_table_iter_next(&iter, &key, &value)) {
			printf("\tdict_in : %s -> %s\n", key, value);
			g_hash_table_insert(*dict_out, key, value);
		}
		
		printf("\n");
		return TRUE;
}

char* thread_out_async(void* arg) {
    DBusGMethodInvocation* async_out = (DBusGMethodInvocation*)arg;

	gchar* str = g_strdup("str async out");

	GArray *array = g_array_new(FALSE, FALSE, sizeof(gint));
	for (int i = 0; i < 10; i++){;
		g_array_append_val(array, i);
	}

	dbus_g_method_return(async_out, str, array);

	g_free(str);
	g_array_free(array, TRUE);
    return NULL;
}

gboolean demo_test_out_async(
    DemoObj *obj, 
    DBusGMethodInvocation* async_out,
    GError **error) {
		printf("demo_test_out_async:\n");
		pthread_t tid;
		if (pthread_create(&tid, NULL, (void*)thread_out_async, async_out) != 0) {
			printf("\tpthread_create error.\n");
		}
		printf("\n");
		return TRUE;
}

gboolean demo_test_error(DemoObj *obj, GError **error) {
	g_set_error(error, G_FILE_ERROR, G_FILE_ERROR_NOENT, "demo_test_error");
	return FALSE;
}

gboolean demo_test_send_signal(
    DemoObj *obj, 
    GError **error) {
		printf("demo_test_send_signal:\n");
		GHashTable* dict = g_hash_table_new(g_str_hash, g_str_equal);
		g_hash_table_insert(dict, g_strdup("key 1"), g_strdup("value 1"));
		g_hash_table_insert(dict, g_strdup("key 2"), g_strdup("value 2"));
		
		g_signal_emit(obj, signals[SIG_TEST_SIGNAL], 0, dict);
		g_hash_table_destroy(dict);
		printf("\n");
		return TRUE;
}
	
int register_to_bus(DBusBusType dbus_type, const char* path, const char* interface, GType obj_type, const DBusGObjectInfo* obj_info) {
	GError *error = NULL;

	DBusGConnection* bus = dbus_g_bus_get(dbus_type, &error); 
	if (!bus) {
		printf("Couldn't connect to session bus : %s\n", error->message);
		return 1;
	}

	DBusGProxy* bus_proxy = dbus_g_proxy_new_for_name(
						bus, 
						"org.freedesktop.DBus",
						"/org/freedesktop/DBus",
						"org.freedesktop.DBus");

	guint request_name_result;
	if (!dbus_g_proxy_call(bus_proxy, "RequestName", &error,
				G_TYPE_STRING, interface,
				G_TYPE_UINT, 0,
				G_TYPE_INVALID,
				G_TYPE_UINT, &request_name_result,
				G_TYPE_INVALID)) {
		printf("Failed to acquire service : %s\n", error->message);
		return 1;
	}

	dbus_g_object_type_install_info(obj_type, obj_info);
	DemoObj* obj = g_object_new(obj_type, NULL);
	dbus_g_connection_register_g_object(bus, path, G_OBJECT(obj));
	return 0;
}

int main() {
#if !GLIB_CHECK_VERSION(2, 36, 0)
    g_type_init (); // glib 2.36之前版本需要调用 g_type_init函数来初始化库GObject type system
#endif

	// DBUS_BUS_SESSION注册到SESSION BUS
	// DBUS_BUS_SYSTEM注册到SYSTEM BUS
	if(register_to_bus(DEMO_DBUS_BUS_TYPE, PATH_NAME, INTERFACE_NAME, OBJECT_TYPE,  &dbus_glib_demo_object_info) != 0) {
		return 1;
	}

	printf ("service running...\n");
	GMainLoop* mainloop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(mainloop);
	return 0;
}
