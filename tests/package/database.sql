CREATE  USER IF NOT EXISTS 'develop'@'%' IDENTIFIED BY '123456';
CREATE DATABASE `muposys-dev`;
GRANT ALL PRIVILEGES ON `muposys-dev`.* TO 'develop'@'%'IDENTIFIED BY '123456';
USE `muposys-dev`;
