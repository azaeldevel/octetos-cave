CREATE  USER IF NOT EXISTS 'develop'@localhost IDENTIFIED BY '123456';
CREATE DATABASE `muposys-dev13`;
GRANT ALL PRIVILEGES ON `muposys-dev13`.* TO 'develop'@'localhost';
USE `muposys-dev13`;
