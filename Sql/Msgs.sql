/*
Navicat MySQL Data Transfer

Source Server         : MariaDB
Source Server Version : 50505
Source Host           : 192.168.0.90:3306
Source Database       : aoyi

Target Server Type    : MYSQL
Target Server Version : 50505
File Encoding         : 65001

Date: 2018-05-18 14:12:40
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `Msgs`
-- ----------------------------
DROP TABLE IF EXISTS `Msgs`;
CREATE TABLE `Msgs` (
  `ID` bigint(20) unsigned NOT NULL,
  `Msg` varchar(1024) NOT NULL,
  UNIQUE KEY `MainIndex` (`ID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of Msgs
-- ----------------------------
INSERT INTO `Msgs` VALUES ('1', '欢迎使用奥医慧学APP。');
