/* This header with some Gstreamer-specific test utilites.*/
#include <gst/check/gstcheck.h>
#include <gst/gst.h>
#include <stdbool.h>

/*Checking setup element*/
static bool check_element (gchar * element_name)
{
    GstElement *element = gst_check_setup_element(element_name);

    if (!element) {
        g_print("Failed to create element - %s\n", element_name);
        return false;
    }

    gst_object_unref(GST_OBJECT(element));
    return true;
}

/* Surrounding tests with GST_START_TEST and GST_END_TEST macros.*/
GST_START_TEST (mtkaacdec_test)
{
    bool test = check_element("mtkaacdec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkac3dec_test)
{
    bool test = check_element("mtkac3dec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkav1dec_test)
{
    bool test = check_element("mtkav1dec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkdtsxp2dec_test)
{
    bool test = check_element("mtkdtsxp2dec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkdvdec_test)
{
    bool test = check_element("mtkdvdec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkeacdec_test)
{
    bool test = check_element("mtkeacdec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkflacdec_test)
{
    bool test = check_element("mtkflacdec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkh264dec_test)
{
    bool test = check_element("mtkh264dec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkh265dec_test)
{
    bool test = check_element("mtkh265dec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkh266dec_test)
{
    bool test = check_element("mtkh266dec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkmjpegdec_test)
{
    bool test = check_element("mtkmjpegdec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkmp3dec_test)
{
    bool test = check_element("mtkmp3dec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkopusdec_test)
{
    bool test = check_element("mtkopusdec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkpcmdec_test)
{
    bool test = check_element("mtkpcmdec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkvorbisdec_test)
{
    bool test = check_element("mtkvorbisdec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkvp8dec_test)
{
    bool test = check_element("mtkvp8dec");
    assert_equals_int(true,test);
}
GST_END_TEST;

GST_START_TEST (mtkvp9dec_test)
{
    bool test = check_element("mtkvp9dec");
    assert_equals_int(1,test);
}
GST_END_TEST;


/* Test sute initialization function for tests registeration. It must end
with "_sute" and traditionally starts with namespace of plugin's.*/
Suite *gst_skeleton_integration_test_suite(void)
{
    Suite *s = suite_create("gst_skeleton_integration_test");
    TCase *tc = tcase_create("general");
    suite_add_tcase(s,tc);
    tcase_add_test (tc, mtkaacdec_test);
    tcase_add_test (tc, mtkac3dec_test);
    tcase_add_test (tc, mtkav1dec_test);
    tcase_add_test (tc, mtkdtsxp2dec_test);
    tcase_add_test (tc, mtkdvdec_test);
    tcase_add_test (tc, mtkeacdec_test);
    tcase_add_test (tc, mtkflacdec_test);
    tcase_add_test (tc, mtkh264dec_test);
    tcase_add_test (tc, mtkh265dec_test);
    tcase_add_test (tc, mtkh266dec_test);
    tcase_add_test (tc, mtkmjpegdec_test);
    tcase_add_test (tc, mtkmp3dec_test);
    tcase_add_test (tc, mtkopusdec_test);
    tcase_add_test (tc, mtkpcmdec_test);
    tcase_add_test (tc, mtkvorbisdec_test);
    tcase_add_test (tc, mtkvp8dec_test);
    tcase_add_test (tc, mtkvp9dec_test);
    //Adding more tests with tcase_add_test(tc,test_function_name)
    return s;
}

/*The entry point for executing test suite. The argument -
name of sute initialization function without "_sute".*/
GST_CHECK_MAIN(gst_skeleton_integration_test);
