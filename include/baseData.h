/*
 * @Author: robox-xx 118261752+robox-xx@users.noreply.github.com
 * @Date: 2023-05-15 19:06:53
 * @LastEditors: robox-xx 118261752+robox-xx@users.noreply.github.com
 * @LastEditTime: 2023-05-17 10:18:04
 * @FilePath: /rc_ws/src/ladar_pkg/include/baseData.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _baseData_
#define _baseData_
#include <math.h>
#define PI acos(-1)

const int DATA_NUM = 1081; //雷达数据个数
const float R = 0.15 ; // 柱半径
const float ANGLE_INCREMENT = (3*PI)/(2*1081);  // 雷达角度增量(3 * PI / 2) / 360
#endif

