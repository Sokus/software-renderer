
#define FUNCTION_SIG(name) void name(float a)
typedef FUNCTION_SIG(FuncTypeA);

typedef void FuncTypeB(float a);
typedef void (*FuncTypeC)(float);


FUNCTION_SIG(TEST)
{
    (void)(a);
}

int main()
{
    FuncTypeB *p = TEST;
    p(5);
    return 0;
}


