/*
Navicat MySQL Data Transfer

Source Server         : MariaDB
Source Server Version : 50505
Source Host           : 192.168.0.90:3306
Source Database       : aoyi

Target Server Type    : MYSQL
Target Server Version : 50505
File Encoding         : 65001

Date: 2018-05-09 10:04:17
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `Students`
-- ----------------------------
DROP TABLE IF EXISTS `Students`;
CREATE TABLE `Students` (
  `Device` varchar(64) DEFAULT '',
  `School` varchar(64) NOT NULL DEFAULT '',
  `Major` varchar(64) DEFAULT '',
  `StudentID` varchar(64) NOT NULL DEFAULT '',
  `Name` varchar(64) DEFAULT NULL,
  `PhoneNumber` varchar(64) DEFAULT '',
  `UserName` varchar(64) DEFAULT '',
  `Password` varchar(64) DEFAULT '',
  `DeviceName` varchar(64) DEFAULT '',
  `DeviceType` varchar(64) DEFAULT '',
  `MsgID` bigint(20) DEFAULT NULL,
  UNIQUE KEY `SearchByStudentInfo` (`School`,`StudentID`) USING BTREE,
  UNIQUE KEY `SearchByAccountInfo` (`UserName`,`Password`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of Students
-- ----------------------------
INSERT INTO `Students` VALUES ('dafe763dc7bc7121b3f83399aff2fe67acb568ed', 'aoyi', '11', '001', '11', '123', '110', '110', 'DESKTOP-N8F2QEI', 'Desktop', null);
INSERT INTO `Students` VALUES ('dafe763dc7bc7121b3f83399aff2fe67acb568ed', 'aoyi', 'sd', '002', 'ds', '123456789101', '120', '111', 'DESKTOP-N8F2QEI', 'Desktop', null);
INSERT INTO `Students` VALUES (null, 'aoyi', null, '003', null, null, null, null, null, null, null);
