
 extern void fsdbTransDPI_scope_add_logicvec_attribute(/* OUTPUT */int *state, /* INPUT */const char* scope_fullname, /* INPUT */const char* attribute_name, const /* INPUT */svLogicVecVal *attribute, /* INPUT */int numbit, /* INPUT */const char* options);

 extern void fsdbTransDPI_scope_add_int_attribute(/* OUTPUT */int *state, /* INPUT */const char* scope_fullname, /* INPUT */const char* attribute_name, /* INPUT */int attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_scope_add_string_attribute(/* OUTPUT */int *state, /* INPUT */const char* scope_fullname, /* INPUT */const char* attribute_name, /* INPUT */const char* attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_scope_add_real_attribute(/* OUTPUT */int *state, /* INPUT */const char* scope_fullname, /* INPUT */const char* attribute_name, /* INPUT */double attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_scope_add_enum_int_attribute(/* OUTPUT */int *state, /* INPUT */const char* scope_fullname, /* INPUT */const char* attribute_name, /* INPUT */unsigned int enum_id, /* INPUT */int attribute, /* INPUT */const char* options);

 extern int fsdbTransDPI_create_stream_begin(/* OUTPUT */int *state, /* INPUT */const char* stream_fullname, /* INPUT */const char* description, /* INPUT */const char* options);

 extern void fsdbTransDPI_define_logicvec_attribute(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* attribute_name, const /* INPUT */svLogicVecVal *attribute, /* INPUT */int numbit, /* INPUT */const char* options);

 extern void fsdbTransDPI_define_int_attribute(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* attribute_name, /* INPUT */int attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_define_string_attribute(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* attribute_name, /* INPUT */const char* attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_define_real_attribute(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* attribute_name, /* INPUT */double attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_define_enum_int_attribute(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* attribute_name, /* INPUT */unsigned int enum_id, /* INPUT */int attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_stream_add_logicvec_attribute(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* attribute_name, const /* INPUT */svLogicVecVal *attribute, /* INPUT */int numbit, /* INPUT */const char* options);

 extern void fsdbTransDPI_stream_add_int_attribute(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* attribute_name, /* INPUT */int attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_stream_add_string_attribute(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* attribute_name, /* INPUT */const char* attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_stream_add_real_attribute(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* attribute_name, /* INPUT */double attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_stream_add_enum_int_attribute(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* attribute_name, /* INPUT */unsigned int enum_id, /* INPUT */int attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_create_stream_end(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* options);

 extern long long fsdbTransDPI_begin(/* OUTPUT */int *state, /* INPUT */int sid, /* INPUT */const char* trans_type, /* INPUT */const char* options);

 extern void fsdbTransDPI_set_label(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* label, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_tag(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* tag, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_logicvec_attribute(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* attribute_name, const /* INPUT */svLogicVecVal *attribute, /* INPUT */int numbit, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_int_attribute(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* attribute_name, /* INPUT */int attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_string_attribute(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* attribute_name, /* INPUT */const char* attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_real_attribute(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* attribute_name, /* INPUT */double attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_enum_int_attribute(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* attribute_name, /* INPUT */unsigned int enum_id, /* INPUT */int attribute, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_logicvec_attribute_with_expected_value(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* attribute_name, const /* INPUT */svLogicVecVal *attribute, /* INPUT */int numbit, const /* INPUT */svLogicVecVal *expected_val, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_int_attribute_with_expected_value(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* attribute_name, /* INPUT */int attribute, /* INPUT */int expected_val, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_string_attribute_with_expected_value(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* attribute_name, /* INPUT */const char* attribute, /* INPUT */const char* expected_val, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_real_attribute_with_expected_value(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* attribute_name, /* INPUT */double attribute, /* INPUT */double expected_val, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_enum_int_attribute_with_expected_value(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* attribute_name, /* INPUT */unsigned int enum_id, /* INPUT */int attribute, /* INPUT */int expected_val, /* INPUT */const char* options);

 extern void fsdbTransDPI_end(/* OUTPUT */int *state, /* INPUT */long long tid, /* INPUT */const char* options);

 extern void fsdbTransDPI_add_relation(/* OUTPUT */int *state, /* INPUT */const char* rel_name, /* INPUT */long long master_tid, /* INPUT */long long slave_tid, /* INPUT */const char* options);

 extern unsigned int fsdbTransDPI_get_enum_id(/* OUTPUT */int *state, /* INPUT */const char* enum_var_name);

 extern SV_STRING fsdbTransDPI_get_class_str(/* OUTPUT */int *state, /* INPUT */const char* class_var_name, /* INPUT */const char* options);
