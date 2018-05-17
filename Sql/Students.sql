/*
Navicat MySQL Data Transfer

Source Server         : MariaDB
Source Server Version : 50505
Source Host           : 192.168.0.90:3306
Source Database       : aoyi

Target Server Type    : MYSQL
Target Server Version : 50505
File Encoding         : 65001

Date: 2018-05-17 17:15:58
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `Students`
-- ----------------------------
DROP TABLE IF EXISTS `Students`;
CREATE TABLE `Students` (
  `Device` varchar(64) DEFAULT '0',
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
  UNIQUE KEY `SearchByStudentInfo` (`School`,`StudentID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of Students
-- ----------------------------
INSERT INTO `Students` VALUES ('dafe763dc7bc7121b3f83399aff2fe67acb568ed', 'aoyi', '11', '000', '11', '1352254685', '110', '130', 'DESKTOP-N8F2QEI', 'Desktop', '36');
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '001', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '002', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '003', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '004', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '005', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '006', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '007', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '008', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '009', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '010', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '011', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '012', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '013', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '014', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '015', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '016', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '017', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '018', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '019', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '020', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '021', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '022', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '023', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '024', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '025', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '026', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '027', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '028', null, '', '', '', '', '', null);
INSERT INTO `Students` VALUES ('0', 'aoyi', '', '029', null, '', '', '', '', '', null);
