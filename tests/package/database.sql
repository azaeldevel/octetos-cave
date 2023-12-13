
CREATE  USER IF NOT EXISTS 'develop'@localhost IDENTIFIED BY '123456';
CREATE DATABASE `muposys-dev`;
GRANT ALL PRIVILEGES ON `muposys-dev`.* TO 'develop'@'localhost';

USE `muposys-dev`;

