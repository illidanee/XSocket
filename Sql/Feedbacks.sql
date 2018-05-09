/*
Navicat MySQL Data Transfer

Source Server         : MariaDB
Source Server Version : 50505
Source Host           : 192.168.0.90:3306
Source Database       : aoyi

Target Server Type    : MYSQL
Target Server Version : 50505
File Encoding         : 65001

Date: 2018-05-09 10:04:11
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `Feedbacks`
-- ----------------------------
DROP TABLE IF EXISTS `Feedbacks`;
CREATE TABLE `Feedbacks` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `UserName` varchar(64) NOT NULL,
  `Content` varchar(1024) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of Feedbacks
-- ----------------------------
INSERT INTO `Feedbacks` VALUES ('13', '你好', '哈哈啊');
INSERT INTO `Feedbacks` VALUES ('14', '你好', '哈哈啊');
INSERT INTO `Feedbacks` VALUES ('15', '你好', '哈哈啊');
INSERT INTO `Feedbacks` VALUES ('16', '你好', '哈哈啊');
INSERT INTO `Feedbacks` VALUES ('17', '你好', '哈哈啊');
INSERT INTO `Feedbacks` VALUES ('18', '你好', '哈哈啊');
INSERT INTO `Feedbacks` VALUES ('19', '你好11', '哈哈啊');
