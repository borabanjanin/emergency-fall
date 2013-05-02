#include <python2.7/Python.h>
#include <stdio.h>

void exec_pycode(const char* py_code);

int main(int argc, char* argv[])
{
     PyObject *myPyrowl, *myPprint, *myOS, *myTime, *myRes, *arglist, *arglist2, *arglist3, *myP = NULL;
     printf("Starting\n");
     Py_Initialize();
     PyRun_SimpleString("import sys");
     PyRun_SimpleString("sys.path.append(\".\")");
     myPyrowl = PyImport_ImportModule("pynma");
     myPprint = PyImport_ImportModule("pprint");
     myOS = PyImport_ImportModule("os");
     myTime = PyImport_ImportModule("time");
     arglist = Py_BuildValue("(s)","b95a1e3705fed5edcb00652f405a768dfef02001c687c1eb"); 
     myP = PyEval_CallObject(PyObject_GetAttrString(myPyrowl, "PyNMA"), arglist);
     myRes = PyObject_GetAttrString(myP, "push");
     arglist2 = Py_BuildValue("(s, s, s, s)","My Raspberry Pi", "Fall Warning", "Patient 1 Has Fallen", "batch_mode=False");
     PyEval_CallObject(myRes, arglist2);
     arglist3 = Py_BuildValue("(i)", 60); /*This really isn't needed, makes the program terminate after 60 sec*/
     PyEval_CallObject(PyObject_GetAttrString(myTime, "sleep"), arglist3);
     Py_Finalize();
     return 0;
}

void exec_pycode(const char* py_code)
{
     Py_Initialize();
     PyRun_SimpleString(py_code);
     Py_Finalize();
}
