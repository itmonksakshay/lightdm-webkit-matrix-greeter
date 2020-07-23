#include<gtk/gtk.h>
#include<webkit/webkit.h>
#include<lightdm.h>
#include<JavaScriptCore/JavaScript.h>
#include<config.h>
static JSValueRef
get_hostname_cb (JSContextRef context,
                 JSObjectRef thisObject,
                 JSStringRef propertyName,
                 JSValueRef *exception)
{
    JSStringRef string;

    string = JSStringCreateWithUTF8CString (lightdm_get_hostname ());

    return JSValueMakeString (context, string);
}

static JSValueRef
can_shutdown_cb (JSContextRef context,
                 JSObjectRef thisObject,
                 JSStringRef propertyName,
                 JSValueRef *exception)
{
	
	return JSValueMakeBoolean(context, lightdm_get_can_shutdown());
}
static JSValueRef
can_restart_cb (JSContextRef context,
                 JSObjectRef thisObject,
                 JSStringRef propertyName,
                 JSValueRef *exception)
{
	
	return JSValueMakeBoolean(context, lightdm_get_can_restart());
}
static JSValueRef
can_sleep_cb (JSContextRef context,
                 JSObjectRef thisObject,
                 JSStringRef propertyName,
                 JSValueRef *exception)
{
	
	return JSValueMakeBoolean(context, lightdm_get_can_suspend());
}
static JSValueRef
can_hibernate_cb (JSContextRef context,
                 JSObjectRef thisObject,
                 JSStringRef propertyName,
                 JSValueRef *exception){
	    
	return JSValueMakeBoolean(context, lightdm_get_can_hibernate());
}

static JSValueRef
shutdown_action (JSContextRef context,
             JSObjectRef function,
             JSObjectRef thisObject,
             size_t argumentCount,
             const JSValueRef arguments[],
             JSValueRef *exception)
{
    // FIXME: Throw exception
    if (argumentCount != 0)
        return JSValueMakeNull (context);

    lightdm_shutdown(NULL);
    return JSValueMakeNull (context);
}

static JSValueRef
restart_action (JSContextRef context,
             JSObjectRef function,
             JSObjectRef thisObject,
             size_t argumentCount,
             const JSValueRef arguments[],
             JSValueRef *exception)
{
    // FIXME: Throw exception
    if (argumentCount != 0)
        return JSValueMakeNull (context);

    lightdm_restart(NULL);
    return JSValueMakeNull (context);
}

static JSValueRef
sleep_action (JSContextRef context,
             JSObjectRef function,
             JSObjectRef thisObject,
             size_t argumentCount,
             const JSValueRef arguments[],
             JSValueRef *exception)
{
    // FIXME: Throw exception
    if (argumentCount != 0)
        return JSValueMakeNull (context);

    lightdm_suspend(NULL);
    return JSValueMakeNull (context);
}

static JSValueRef
hibernate_action (JSContextRef context,
             JSObjectRef function,
             JSObjectRef thisObject,
             size_t argumentCount,
             const JSValueRef arguments[],
             JSValueRef *exception)
{
    // FIXME: Throw exception
    if (argumentCount != 0)
        return JSValueMakeNull (context);

    lightdm_hibernate(NULL);
    return JSValueMakeNull (context);
}

static JSValueRef user_validate(JSContextRef context,
	     JSObjectRef function,
             JSObjectRef thisObject,
             size_t argumentCount,             
             const JSValueRef arg[],
             JSValueRef *exception) {
	size_t size;
	JSStringRef string;
	gchar *username;

    // FIXME: Throw exception
    if ((argumentCount != 1) && (JSValueGetType(context, arg[0]) != kJSTypeString)){
    	return JSValueMakeNull(context);

    }
	string = JSValueToStringCopy(context,arg[0],exception);

	if (!string) {

		return NULL;
	}

	size = JSStringGetMaximumUTF8CStringSize(string);
	username = g_malloc(size);

	if (!username) {

		return NULL;
	}

	JSStringGetUTF8CString(string,username,size);
	JSStringRelease(string);

	if(lightdm_user_list_get_user_by_name(lightdm_user_list_get_instance(),username) != NULL
			){
		return JSValueMakeBoolean(context,true);
	}
	return JSValueMakeBoolean(context,false);        
}



static const JSStaticValue lightdm_greeter_values[] =
{
    { "hostname", get_hostname_cb, NULL, kJSPropertyAttributeReadOnly },
    { "can_shutdown", can_shutdown_cb, NULL, kJSPropertyAttributeReadOnly },
    { "can_restart", can_restart_cb, NULL, kJSPropertyAttributeReadOnly },
    { "can_sleep", can_sleep_cb, NULL, kJSPropertyAttributeReadOnly },
    { "can_hibernate", can_hibernate_cb, NULL, kJSPropertyAttributeReadOnly },
    {NULL,NULL,NULL,0}

};

static const JSStaticFunction lightdm_command_functions[] =
{
	{"shutdown",shutdown_action,kJSPropertyAttributeReadOnly},
	{"restart",restart_action,kJSPropertyAttributeReadOnly},
	{"sleep",sleep_action,kJSPropertyAttributeReadOnly},
	{"hibernate",hibernate_action,kJSPropertyAttributeReadOnly},	
	{"userValidate",user_validate,kJSPropertyAttributeReadOnly},
	{NULL,NULL,0}
};

static const JSClassDefinition lightdm_greeter_definition =
{
    0,                     /* Version */
    kJSClassAttributeNone, /* Attributes */
    "LightDMGreeter",          /* Class name */
    NULL,                  /* Parent class */
    lightdm_greeter_values,    /* Static values */
    lightdm_command_functions,
};


static void
window_object_cleared_cb (WebKitWebView  *web_view,
                          WebKitWebFrame *frame,
                          JSGlobalContextRef context,
                          JSObjectRef window_object,
                          LightDMGreeter *greeter)
{
    JSClassRef classDef = JSClassCreate(&lightdm_greeter_definition);
    JSObjectRef classObj = JSObjectMake(context, classDef, context);
    JSObjectRef globalObj = JSContextGetGlobalObject(context);
    JSStringRef str = JSStringCreateWithUTF8CString("lightdm");
    JSObjectSetProperty(context, globalObj, str, classObj, kJSPropertyAttributeNone, NULL);


   }

static void show_prompt_cb(){



}

static void authentication_complete_cb(){



}





int main(int argc,char *argv[]){

	GdkRectangle geometry;
	GKeyFile *config;
	gchar *value;
	static LightDMGreeter *greeter;
	
	gtk_init(&argc,&argv);

	/*config = g_key_file_new ();
   	g_key_file_load_from_file (config, CONFIG_FILE, G_KEY_FILE_NONE, &error);
    	if (error && !g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT)){
        	g_warning ("Failed to load configuration from %s: %s\n", 
				CONFIG_FILE, error->message);
	}
    	g_clear_error (&error);*/

	
	gdk_window_set_cursor(gdk_get_default_root_window(),
			    gdk_cursor_new_for_display(gdk_display_get_default(), 
				    GDK_LEFT_PTR));

	GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL,NULL);
	GtkWidget *web_view = webkit_web_view_new();
    	g_signal_connect (G_OBJECT (web_view), "window-object-cleared", 
			G_CALLBACK(window_object_cleared_cb), web_view);
	gtk_container_add(GTK_CONTAINER(scrolled_window),web_view);
	gtk_container_add(GTK_CONTAINER(main_window),scrolled_window);
	webkit_web_view_load_uri (WEBKIT_WEB_VIEW (web_view), 
			"file:///Data/Webkit_gtk_development/lightdm_webkit_greeter/themes/default/index.html");
	
	gdk_monitor_get_geometry(gdk_display_get_primary_monitor(gdk_display_get_default()), 
			&geometry);
    	gtk_window_set_default_size(GTK_WINDOW(main_window),geometry.width,geometry.height);
	greeter = lightdm_greeter_new();
	g_signal_connect (greeter, "show-prompt", G_CALLBACK (show_prompt_cb), NULL);
    	g_signal_connect (greeter, "authentication-complete",
		       	G_CALLBACK (authentication_complete_cb), NULL);

	
    	gtk_widget_show_all (main_window);
	gtk_main();
return 0;
}
