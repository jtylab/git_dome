/**
 * @file FFC.h
 * @author jiang
 * @brief 前馈控制
 * @version 0.1
 * @date 2025-04-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef FFC_H
 #define FFC_H

 #include "main.h"
 
 #ifdef __cplusplus
 extern "C" {
 #endif

 #ifdef __cplusplus

class FFC_t{
    private:
        float Kf;
        float Target_t0;
        float Target_t1;
        float Target_ROC;

    public:
        FFC_t(){
            Target_t0 = 0;
            Target_t1 = 0;
            Target_ROC = 0;
            Kf = 0;
        }
        void Set_Kf(float Kf);
        void Generate(float Target);
        float GetTatget_ROC(void);
        float GetKf(void);

};


#endif



#ifdef __cplusplus
} /* extern "C" */
#endif
 
 #endif // !FFC_H