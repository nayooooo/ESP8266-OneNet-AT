#include "at.h"

At_Class::At_Class(const At_State_t atTable)
{
    _atTable = atTable;
    _param_max_nun = AT_PARAM_MAX_NUM;
}

At_Err_t At_Class::At_Cut_String(struct At_Param &param, const String &atLable)
{
    char *str = (char*)(atLable.c_str());

    param.cmd = AT_LABLE_TAIL;
    param.argc = 0;
    for (int i = 0; i < At_Get_Param_Max_Num(); i++)
        param.argv[i] = (char*)"";

    // find at lable
    param.cmd = strtok(str, " ");
    // find at param
    for (int i = 0; i < At_Get_Param_Max_Num(); i++) {
        param.argv[i] = strtok(NULL, " ");
        if (param.argv[i] == NULL) break;
        param.argc++;
    }
    
    return AT_OK;
}

At_State_t At_Class::At_Check_String(struct At_Param &param, const At_State_t atTable, const String &atLable)
{
    uint32_t i = 0;
    At_State_t target = NULL;

    At_Cut_String(param, atLable);
    
    while (atTable[i].atLable != AT_LABLE_TAIL) {
        if (atTable[i].atLable == param.cmd) {
            target = &atTable[i];
            break;
        }
        i++;
    }

    return target;
}

String At_Class::At_ErrorToString(At_Err_t error)
{
    switch (error)
    {
    case AT_ERROR: return String("AT normal error");
    case AT_ERROR_NOT_FIND: return String("AT not find this string command");
    case AT_ERROR_NO_ACT: return String("AT this string command not have act");
    case AT_ERROR_CANNOT_CUT: return String("AT this string can't be cut");
    }
    return String("AT no error");
}

int At_Class::At_Get_Param_Max_Num(void)
{
    return _param_max_nun;
}

At_Err_t At_Class::At_Handle(const String &atLable)
{
    struct At_Param param;
    At_State_t target = At_Check_String(param, _atTable, atLable);

    if (target == NULL) return AT_ERROR_NOT_FIND;
    if (target->act == NULL) return AT_ERROR_NO_ACT;

    At_Err_t ret = target->act(param.argc, param.argv);
    if (ret != AT_OK) return AT_ERROR;
    
    return AT_OK;
}
