/*
Navicat MySQL Data Transfer

Source Server         : MariaDB
Source Server Version : 50505
Source Host           : 192.168.0.90:3306
Source Database       : aoyi

Target Server Type    : MYSQL
Target Server Version : 50505
File Encoding         : 65001

Date: 2018-05-18 14:12:46
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
  `MsgID` bigint(20) unsigned NOT NULL DEFAULT 0,
  UNIQUE KEY `SearchByStudentInfo` (`School`,`StudentID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of Students
-- ----------------------------
INSERT INTO `Students` VALUES ('0fff5d25f1f4e0f3a5d9698ebb8e9ed8', '奥医科技', '-', '000', '张亮', '99999999999', '999', '999', '<unknown>', 'Handheld', '36');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '001', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '002', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '003', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '004', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '005', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '006', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '007', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '008', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '009', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '010', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '011', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '012', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '013', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '014', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '015', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '016', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '017', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '018', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '019', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '020', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '021', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '022', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '023', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '024', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '025', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '026', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '027', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '028', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('0', '奥医科技', '-', '029', null, '', '', '', '', '', '0');
INSERT INTO `Students` VALUES ('dafe763dc7bc7121b3f83399aff2fe67acb568ed', '奥医科技', '-', '100', '11', '1352254685', '110', '130', 'DESKTOP-N8F2QEI', 'Desktop', '0');
