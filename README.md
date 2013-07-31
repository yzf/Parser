Parser
======
命名规范：
    1. 函数    小写字母，单词间用下划线
            void test_me();
    2. 类名    大驼峰式命名法
            class Point {};
    3. 类函数  小驼峰式命名法
            参数：下划线+小驼峰式命名法
            class Point {
                    void testMe();
            }
    4. 类变量
            a) 格式：[m_|s_|g_] type [class_name|struct_name] variable_name
            b) 解释：
                    m_ ： 类的成员变量
                    ms_： 类的静态成员变量
                    s_ ： 静态全局变量
                    g_ ： 普通全局变量
            c) 类型缩写：
                    char, TCHAR： ch
                    char[]，TCHAR[]： sz
                    bool： b
                    int： n
                    long： l
                    double： d
                    float： ft
                    unsigned： u
                    p ：pointer
            class Point {
            private:
                    int m_nSize;
                    static int ms_nCount;
            };

    5. 宏	大写字母，单词间使用下划线
            #define MAX_NUM 100

    6. 联合体 uni_ type [class_name|struct_name] variable_name
            union Variant {
                    char uni_chVal;
                    int uni_nVal;
                    float uni_ftVal;
            }

    7. 局部变量	type [class_name|struct_name] variable_name
            int nCount;
            若是函数参数，则加下划线
            void test_me(int _nCount);
    8. 枚举类型  大写，单词间下划线
        enum TERM_TYPE {
            VARI = 0,
            FUNC,
        };


by yzf - -!
