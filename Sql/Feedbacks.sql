/*
Navicat MySQL Data Transfer

Source Server         : MariaDB
Source Server Version : 50505
Source Host           : 192.168.0.90:3306
Source Database       : aoyi

Target Server Type    : MYSQL
Target Server Version : 50505
File Encoding         : 65001

Date: 2018-05-04 17:46:28
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `Feedbacks`
-- ----------------------------
DROP TABLE IF EXISTS `Feedbacks`;
CREATE TABLE `Feedbacks` (
  `UserName` varchar(64) NOT NULL,
  `Content` varchar(1024) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of Feedbacks
-- ----------------------------
INSERT INTO `Feedbacks` VALUES ('你好', '哈哈啊');
INSERT INTO `Feedbacks` VALUES ('你好', '哈哈啊');
INSERT INTO `Feedbacks` VALUES ('你好', '哈哈啊');
INSERT INTO `Feedbacks` VALUES ('你好', '哈哈啊');
INSERT INTO `Feedbacks` VALUES ('你好', '哈哈啊');
