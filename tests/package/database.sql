CREATE  USER IF NOT EXISTS 'develop'@localhost IDENTIFIED BY '123456';
CREATE DATABASE `muposys-dev15`;
GRANT ALL PRIVILEGES ON `muposys-dev15`.* TO 'develop'@'localhost';
USE `muposys-dev15`;
