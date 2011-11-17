/*
 * parser.cpp
 *
 *  Created on: Mar 17, 2009
 *  	Author: Phoebus Veiz <phoebusveiz@gmail.com>
 */

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "def.h"
#include "util.h"
#include "parser.h"

static char forbidden[][3] = {
	{-52, -41, 0},	// 套
	{-44, -95, 0},	// 浴
	{-48, -44, 0},  // 性
	{-53, -64, 0},  // 死
	{-26, -50, 0},  // 嫖
	{-46, -7, 0},   // 淫
	{-59, -82, 0},  // 女
	{-68, -23, 0},  // 奸
	{-60, -48, 0},  // 男
	{-50, -57, 0},  // 吻
	{-79, -89, 0},  // 抱
	{-58, -34, 0},  // 妻
	{-80, -82, 0},  // 爱
	{-69, -23, 0},  // 婚
	{-55, -89, 0},  // 骚
	{-68, -6, 0},   // 贱
	{-51, -47, 0},  // 脱
	{-51, -10, 0},  // 亡
	{-72, -66, 0},  // 妇
	{-56, -94, 0},  // 娶
	{-68, -34, 0},  // 嫁
	{-55, -79, 0},  // 杀
	{-55, -19, 0},  // 身
	{-52, -27, 0},  // 体
	{-56, -23, 0},  // 乳
	{-51, -50, 0},  // 臀
	{-55, -85, 0},  // 色
	{-45, -43, 0},  // 诱
	{-58, -83, 0},  // 骗
	{-46, -62, 0},  // 衣
	{-48, -40, 0},  // 胸
	{-60, -93, 0},  // 模
	{-71, -85, 0},  // 公
	{-47, -4, 0},   // 腰
	{-52, -91, 0},  // 胎
	{-57, -23, 0},  // 情
	{-68, -92, 0},  // 激
	{-76, -14, 0},  // 打
	{-79, -87, 0},  // 暴
	{-78, -48, 0},  // 残
	{-77, -79, 0},  // 潮
	{-69, -74, 0},  // 欢
	{-62, -74, 0},  // 露
	{-68, -95, 0},  // 肌
	{-74, -66, 0},  // 毒
	{-63, -75, 0},  // 恋
	{-73, -24, 0},  // 疯
	{-60, -72, 0},  // 母
	{-58, -83, 0},  // 骗
	{-51, -64, 0},  // 屠
	{-47, -86, 0},  // 血
	{-60, -90, 0},  // 摩
	{-71, -85, 0},  // 公
	{-47, -4, 0},   // 腰
	{-76, -91, 0},  // 触
	{-25, -77, 0},  // 绯
	{-56, -30, 0},  // 肉
	{-59, -83, 0},  // 怒
	{-45, -5, 0},   // 欲
	{-50, -24, 0},  // 舞
	{-62, -18, 0},  // 骂
	{-57, -82, 0},  // 钱
	{-62, -29, 0},  // 裸
	{-52, -69, 0},  // 袒
	{-44, -48, 0},  // 孕
	{-41, -77, 0},  // 壮
	{-55, -10, 0},  // 肾
	{-61, -61, 0},  // 妹
	{-64, -79, 0},  // 辣
	{-46, -11, 0},  // 阴
	{-71, -38, 0},  // 冠
	{-67, -61, 0},  // 矫
	{-61, -91, 0},  // 氓
	{-76, -42, 0},  // 粗
	{-48, -71, 0},  // 泄
	{-53, -3, 0},   // 她
	{-62, -47, 0},  // 卵
	{-66, -85, 0},  // 精
	{-76, -67, 0},  // 唇
	{-41, -84, 0},  // 赚
	{-64, -53, 0},  // 浪
	{-53, -89, 0},  // 帅
	{-58, -40, 0},  // 曝
	{-41, -79, 0},  // 妆
	{-67, -29, 0},  // 姐
	{-67, -32, 0},  // 洁
	{-15, -79, 0},  // 癖
	{-65, -31, 0},  // 酷
	{-61, -40, 0},  // 秘
	{-58, -75, 0},  // 频
	{-63, -60, 0},  // 聊
	{-59, -6, 0},   // 批
	{-51, -76, 0},  // 痛
	{-77, -76, 0},  // 炒
	{-64, -10, 0},  // 丽
	{-45, -75, 0},  // 拥
	{-46, -71, 0},  // 夜
	{-76, -78, 0},  // 床
	{-61, -33, 0},  // 眠
	{-61, -44, 0},  // 迷
	{-77, -24, 0},  // 宠
	{-44, -75, 0},  // 缘
	{-56, -31, 0},  // 柔
	{-53, -81, 0},  // 睡
	{-74, -57, 0},  // 肚
	{-49, -75, 0},  // 系
	{-72, -48, 0},  // 感
	{-73, -12, 0},  // 肤
	{-61, -37, 0},  // 蜜
	{-54, -20, 0},  // 熟
	{-61, -32, 0},  // 绵
	{-78, -8, 0},   // 缠
	{-57, -41, 0},  // 亲
	{-52, -46, 0},  // 桃
	{-26, -62, 0},  // 媛
	{-47, -37, 0},  // 眼
	{-27, -48, 0},  // 逍
	{-28, -20, 0},  // 潇
	{-49, -89, 0},  // 惜
	{-61, -4, 0},   // 命
	{-68, -78, 0},  // 疾
	{-78, -95, 0},  // 病
	{-65, -77, 0},  // 砍
	{-75, -74, 0},  // 刀
	{-44, -41, 0},  // 宰
	{-70, -64, 0},  // 豪
	{-71, -13, 0},  // 贵
	{-72, -69, 0},  // 富
	{-78, -58, 0},  // 财
	{-53, -5, 0},   // 他
	{-22, -45, 0},  // 暧
	{-61, -63, 0},  // 昧
	{-71, -20, 0},  // 轨
	{-73, -14, 0},  // 夫
	{-72, -18, 0},  // 割
	{-72, -89, 0},  // 抚
	{-60, -52, 0},  // 奶
	{-33, -28, 0},  // 咪
	{-47, -34, 0},  // 艳
	{-47, -43, 0},  // 颜
	{-61, -78, 0},  // 貌
	{-74, -23, 0},  // 堕
	{-54, -84, 0},  // 尸
	{-60, -71, 0},  // 墓
	{-68, -64, 0},  // 祭
	{-75, -63, 0},  // 盗
	{-57, -64, 0},  // 抢
	{-67, -39, 0},  // 劫
	{-58, -37, 0},  // 欺
	{-62, -66, 0},  // 戮
	{-56, -92, 0},  // 趣
	{-71, -10, 0},  // 滚
	{-62, -24, 0},  // 妈
	{-51, -75, 0},  // 偷
	{-16, -79, 0},  // 鸨
	{-68, -90, 0},  // 鸡
	{-46, -80, 0},  // 野
	{-59, -71, 0},  // 殴
	{-51, -79, 0},  // 捅
	{-80, -13, 0},  // 绑
	{-34, -49, 0},  // 尴
	{-48, -80, 0},  // 邪
	{-78, -39, 0},  // 操
	{-53, -84, 0},  // 爽
	{-30, -85, 0},  // 猥
	{-39, -12, 0},  // 亵
	{-69, -58, 0},  // 黄
	{-69, -20, 0},  // 混
	{-50, -22, 0},  // 侮
	{-56, -24, 0},  // 辱
	{-27, -30, 0},  // 邂
	{-27, -53, 0},  // 逅
	{-65, -15, 0},  // 狂
	{-78, -71, 0},  // 补
	{-71, -76, 0},  // 勾
	{-41, -80, 0},  // 装
	{-45, -23, 0},  // 娱
	{-76, -63, 0},  // 戳
	{-78, -27, 0},  // 插
	{-58, -14, 0},  // 乞
	{-48, -41, 0},  // 凶
	{-71, -19, 0},  // 鬼
	{-41, -23, 0},  // 组
	{-67, -5, 0},   // 禁
	{-57, -44, 0},  // 窃
	{-77, -44, 0},  // 吃
	{-70, -56, 0},  // 喝
	{-75, -33, 0},  // 颠
	{-65, -29, 0},  // 裤
	{-56, -71, 0},  // 裙
	{-65, -6, 0},   // 窥
	{-70, -34, 0},  // 恨
	{-74, -25, 0},  // 剁
	{-48, -33, 0},  // 羞
	{0, 0, 0},
};

bool filter(const string& str)
{
	string::size_type pos;
	size_t kanji_sum = 0;
	for (int i = 0; strlen(forbidden[i]); i++)
	{
		kanji_sum = 0;
		pos = str.find(forbidden[i]);
		if (pos < str.length())
		{
			for (size_t j = 0; j < pos; j++)
			{
				if (str[j] < 0)
					kanji_sum++;
			}
			if (kanji_sum % 2 == 0)
			{
				return false;
			}
		}
	}
	return true;
}

int parser::parse_xml_group(const vector<string>& urls, vector<string>& vec)
{
	const char* local_filename = "title.xml";
	int ret = 0;
	vec.clear();
	for (size_t i = 0; i < urls.size(); i++)
	{
		download(urls.at(i).c_str(), local_filename);
		ret += parser::parse_xml(local_filename, vec);
	}
	std::sort(vec.begin(), vec.end());
	return ret;
}


int parser::parse_xml(const char* xml_file, vector<string>& vec)
{
	const string prefix = "<title><![CDATA[";
	const string suffix = "]]></title>";
	char rm[5] = {40, -51, -68, 41, 0};	// (图)
	const string drop_str = rm;
	const uint32_t len_min = 20;
	ifstream ifs;
	string line;
	string::size_type pos;
	uint32_t mode = 1;
	uint32_t nloop = 0;

	ifs.open(xml_file, ios::in | ios::binary);

	if (!ifs)
	{
		cout << xml_file << ": file not found." << endl;
	}

	start:
	ifs.seekg(0);

	for (; ;)
	{
		nloop++;

		if (nloop > 1024)
		{
			return -1;
		}

		line.clear();

		if (mode == 1)
		{
			ifs >> line;
		}
		else if (mode == 2)
		{
			getline(ifs, line);
		}

		if (line.size() <= 0)
		{
			break;
		}

		pos = line.find(prefix);

		if (pos < line.length())
		{
			line = line.substr(pos +
							prefix.length(),
							line.length() -
							prefix.length() -
							suffix.length() -
							pos);

			pos = line.find(drop_str);

			if (pos < line.length())
			{
				line.erase(pos, drop_str.length());
			}

			if (line.length() > len_min)
			{
				if (filter(line) == true)
					vec.push_back(line);
			}
		}
	}

	if (mode == 1 && vec.empty())
	{
		mode = 2;
		goto start;
	}

	ifs.close();
	return 0;
}
