/*
 * @Author: robox-xx 118261752+robox-xx@users.noreply.github.com
 * @Date: 2023-05-15 19:06:53
 * @LastEditors: robox-xx 118261752+robox-xx@users.noreply.github.com
 * @LastEditTime: 2023-05-16 22:47:26
 * @FilePath: /rc_ws/src/ladar_pkg/include/filter_my.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <math.h>
#include <vector>
#include "baseData.h"
#include "find_center.h"

class Filter
{
    public:
        Filter();
        ~Filter();
        
        void delete_outlier(std::vector<float>&data,std::vector<float>&theta,float radius,int k);
        void get_circle(std::vector<float>&data,std::vector<float>&theta,float deviation);
        void num_less_filter(std::vector<float>&data,const int MinNumber);
        void num_more_filter(std::vector<float>&data,const int MaxNumber);

        void dis_longer_filter(std::vector<float>&data,const int maxdis);
        void dis_shorter_filter(std::vector<float>&data,const int mindis);
        void out_range_points(std::vector<float>&data,std::vector<float>&theta,float theth_res);
    private:
};
