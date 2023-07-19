
CREATE  USER IF NOT EXISTS 'develop'@localhost IDENTIFIED BY '123456';
CREATE DATABASE `muposys-cave` character set ASCII collate ASCII_BIN;
GRANT ALL PRIVILEGES ON `muposys-cave`.* TO 'develop'@'localhost';

USE `muposys-cave`;

