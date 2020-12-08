#include "demo.h"
#include "demo_client_glue.h"
#include <dbus/dbus-glib.h>
#include <stdio.h>

static void handler_test_signal(DBusGProxy *proxy, GHashTable *table, gpointer user_data)
{
    printf("handler_test_signal: \n");
    GHashTableIter iter;
    g_hash_table_iter_init(&iter, table);
    gpointer key, value;
    while(g_hash_table_iter_next(&iter, &key, &value)) {
        printf("\t%s -> %s\n", key, value);
    }
}

DBusGProxy* connect_server(DBusBusType dbus_type, const char* path, const char* interface) {
    GError* error = NULL;

    //和SESSION BUS建立连接
    DBusGConnection* bus = dbus_g_bus_get(dbus_type, &error);
    if (!bus) {
        printf("Couldn't connect to session bus: %s", error->message);
        return NULL;
    }
    //获取一个对象代理
    return dbus_g_proxy_new_for_name (
                                    bus,
                                    interface,
                                    path,
                                    interface);
}

void test_me_linjw_dbus_demo_test_sample_type_in_out(DBusGProxy* remote_object) {
    GError *error = NULL;

    const gboolean IN_bool_in = FALSE;
    const gdouble IN_double_in = 1.1;
    const gint IN_int32_in = 2;
    const gint64 IN_int64_in = 3;
    const guchar IN_uint8_in = 4;
    const guint IN_uint32_in = 5;
    const guint64 IN_uint64_in = 6;
    const char* IN_str_in = "7";
    gboolean OUT_bool_out;
    gdouble OUT_double_out;
    gint OUT_int32_out;
    gint64 OUT_int64_out;
    guchar OUT_uint8_out;
    guint OUT_uint32_out;
    guint64 OUT_uint64_out;
    char* OUT_str_out;
    if(me_linjw_dbus_demo_test_sample_type_in_out(
                remote_object, 
                IN_bool_in, IN_double_in, IN_int32_in, IN_int64_in, IN_uint8_in, IN_uint32_in, IN_uint64_in, IN_str_in,
                &OUT_bool_out, &OUT_double_out, &OUT_int32_out, &OUT_int64_out, &OUT_uint8_out, &OUT_uint32_out, &OUT_uint64_out, &OUT_str_out,
                &error)) {

        printf("me_linjw_dbus_demo_test_sample_type_in_out:\n");
        printf("\tOUT_bool_out = %d\n", OUT_bool_out);
        printf("\tOUT_double_out = %lf\n", OUT_double_out);
        printf("\tOUT_int32_out = %d\n", OUT_int32_out);
        printf("\tOUT_int64_out = %d\n", OUT_int64_out);
        printf("\tOUT_uint8_out = %d\n", OUT_uint8_out);
        printf("\tOUT_uint32_out = %d\n", OUT_uint32_out);
        printf("\tOUT_uint64_out = %d\n", OUT_uint64_out);
        printf("\tOUT_str_out = %s\n", OUT_str_out);
        printf("\n");
    } else {
        printf("call me_linjw_dbus_demo_test_sample_type_in_out failed!!!\n\n");
    }
}

void test_me_linjw_dbus_demo_test_out_async(DBusGProxy* remote_object) {
    GError *error = NULL;

    char* async_out = NULL;
    if(me_linjw_dbus_demo_test_out_async(remote_object, &async_out, &error)) {
        printf("me_linjw_dbus_demo_test_out_async:\n");
        printf("\tasync_out = %s\n", async_out);
        printf("\n");
    } else {
        printf("call me_linjw_dbus_demo_test_out_async failed!!!\n\n");
    }
}

void test_me_linjw_dbus_demo_test_array_in_out(DBusGProxy* remote_object) {
    GError *error = NULL;

    GArray* array_bool_in = g_array_new(FALSE, FALSE, sizeof(gboolean));
    for(int i = 0 ; i<8 ; i++) {
        gboolean b = (i & 1)==0;
        g_array_append_val(array_bool_in, b);
    }

    gchar** array_str_in = malloc(3 * sizeof(gchar*));
    array_str_in[0] = g_strdup("str 1");
    array_str_in[1] = g_strdup("str 2");
    array_str_in[2] = NULL;

    GArray* array_bool_out;
    gchar** array_str_out;
    if(me_linjw_dbus_demo_test_array_in_out(
            remote_object, 
            array_bool_in,
            array_str_in,
            &array_bool_out,
            &array_str_out,
            error)) {
        
        printf("call me_linjw_dbus_demo_test_array_in_out:\n");
        for(int i = 0 ; i<array_bool_out->len ; i++) {
			gboolean b = g_array_index(array_bool_out, gboolean, i);
			printf("\tarray_bool_out %d : %s\n", i, b ? "True" : "False");
		}
        g_array_free(array_bool_out, FALSE);

        int index = 0;
        gchar** it = array_str_out;
		while(*it != NULL) {
			printf("\tarray_str_out %d : %s\n", index, *it);
			index++;
            it++;
		}
        g_strfreev(array_str_out);
        printf("\n");

    } else {
        printf("call me_linjw_dbus_demo_test_array_in_out failed!!!\n\n");
    }
}

void test_me_linjw_dbus_demo_test_value_in_out(DBusGProxy* remote_object) {
    GError *error = NULL;
    
    GValue value_in = {0};
    g_value_init(&value_in, G_TYPE_STRING);
	g_value_set_string(&value_in, g_strdup("hello"));

    GValue value_out = {0};
    if(me_linjw_dbus_demo_test_value_in_out(remote_object, &value_in, &value_out, &error)) {
        printf("call me_linjw_dbus_demo_test_value_in_out:\n");
        printf("\tvalue_out : %s\n", g_value_get_string(&value_out));
        printf("\n");
    } else {
        printf("call me_linjw_dbus_demo_test_value_in_out failed!!!\n\n");
    }
}

void test_me_linjw_dbus_demo_test_struct_in_out(DBusGProxy* remote_object) {
    GError *error = NULL;

    GValueArray* struct_in = g_value_array_new(2);

	g_value_array_append(struct_in, NULL); 
    GValue* value_in = g_value_array_get_nth(struct_in, 0);
    g_value_init(value_in, G_TYPE_INT);
    g_value_set_int(value_in, 123);
    
	g_value_array_append(struct_in, NULL);
    value_in = g_value_array_get_nth(struct_in, 1);
    g_value_init(value_in, G_TYPE_STRING);
    g_value_set_string(value_in, g_strdup("abc"));

    GValueArray* struct_out;
    if(me_linjw_dbus_demo_test_struct_in_out(remote_object, struct_in, &struct_out, &error)) {
        printf("call me_linjw_dbus_demo_test_struct_in_out:\n");
        printf("\tstruct_out i : %d\n", g_value_get_int(g_value_array_get_nth(struct_out,0)));
        printf("\tstruct_out s : %s\n", g_value_get_string(g_value_array_get_nth(struct_out,1)));
        printf("\n");
    } else {
        printf("call me_linjw_dbus_demo_test_struct_in_out failed!!!\n\n");
    }

    g_value_array_free(struct_in);
    g_value_array_free(struct_out);
}

void test_me_linjw_dbus_demo_test_struct_array_in_out(DBusGProxy* remote_object) {
    GError *error = NULL;

    GPtrArray* array_in = g_ptr_array_new();

    GValueArray* struct_in = g_value_array_new(2);

	g_value_array_append(struct_in, NULL); 
    GValue* value_in = g_value_array_get_nth(struct_in, 0);
    g_value_init(value_in, G_TYPE_INT);
    g_value_set_int(value_in, 123);
    
	g_value_array_append(struct_in, NULL);
    value_in = g_value_array_get_nth(struct_in, 1);
    g_value_init(value_in, G_TYPE_STRING);
    g_value_set_string(value_in, g_strdup("abc"));

    g_ptr_array_add(array_in, struct_in);

    GPtrArray* array_out = NULL;
    if(me_linjw_dbus_demo_test_struct_array_in_out(remote_object, array_in, &array_out, &error)) {
        printf("call me_linjw_dbus_demo_test_struct_array_in_out:\n");
        GValueArray* struct_out = g_ptr_array_index(array_out, 0);
        printf("\tstruct_out index 0, i : %d\n", g_value_get_int(g_value_array_get_nth(struct_out,0)));
        printf("\tstruct_out index 0, s : %s\n", g_value_get_string(g_value_array_get_nth(struct_out,1)));
        printf("\n");
    } else {
        printf("call me_linjw_dbus_demo_test_struct_in_out failed!!!\n\n");
    }

    g_ptr_array_free(array_in, TRUE);
    g_ptr_array_free(array_out, TRUE);
}

void test_me_linjw_dbus_demo_test_dict_in_out(DBusGProxy* remote_object) {
    GError *error = NULL;

    GHashTable* dict_in = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(dict_in, g_strdup("key 1"), g_strdup("value 1"));
    g_hash_table_insert(dict_in, g_strdup("key 2"), g_strdup("value 2"));

    GHashTable* dict_out;
    if (me_linjw_dbus_demo_test_dict_in_out(remote_object, dict_in, &dict_out, &error)){
        printf("call me_linjw_dbus_demo_test_dict_in_out:\n");

        GHashTableIter iter;
		g_hash_table_iter_init(&iter, dict_out);
		gpointer key, value;
		while(g_hash_table_iter_next(&iter, &key, &value)) {
			printf("\tdict_out: %s -> %s\n", key, value);
		}
        printf("\n");
    } else {
        printf("call me_linjw_dbus_demo_test_dict_in_out failed!!!\n\n");
    }

    g_hash_table_destroy(dict_in);
    g_hash_table_destroy(dict_out);
}
	
int main() {
    DBusGProxy* remote_object = connect_server(DEMO_DBUS_BUS_TYPE, PATH_NAME, INTERFACE_NAME);

    dbus_g_proxy_add_signal(remote_object, "test_signal", dbus_g_type_get_map("GHashTable", G_TYPE_STRING, G_TYPE_STRING), G_TYPE_INVALID);
    dbus_g_proxy_connect_signal(remote_object, "test_signal", G_CALLBACK(handler_test_signal), NULL, NULL);
    
    test_me_linjw_dbus_demo_test_sample_type_in_out(remote_object);
    test_me_linjw_dbus_demo_test_out_async(remote_object);
    test_me_linjw_dbus_demo_test_array_in_out(remote_object);
    test_me_linjw_dbus_demo_test_value_in_out(remote_object);
    test_me_linjw_dbus_demo_test_struct_in_out(remote_object);
    test_me_linjw_dbus_demo_test_struct_array_in_out(remote_object);
    test_me_linjw_dbus_demo_test_dict_in_out(remote_object);

    GError *error = NULL;
    me_linjw_dbus_demo_test_send_signal(remote_object, &error);

    GMainLoop* mainloop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(mainloop);

    g_object_unref(G_OBJECT(remote_object));
    return 0;
}