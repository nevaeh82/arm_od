SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

DROP SCHEMA IF EXISTS `UAV` ;
CREATE SCHEMA `UAV` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `UAV` ;

-- -----------------------------------------------------
-- Table `UAV`.`UAVTypes`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`UAVTypes` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `typeId` (`id` ASC))
ENGINE = InnoDB;

-- -----------------------------------------------------
-- Table `UAV`.`UAVRoles`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`UAVRoles` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `code` TEXT NOT NULL,
  `name` TEXT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `roleId` (`id` ASC))
ENGINE = InnoDB;

-- -----------------------------------------------------
-- Table `UAV`.`UAV`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`UAV` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `uavId` INT UNSIGNED NOT NULL,
  `ip` TEXT NOT NULL,
  `uavTypeId` INT NULL,
  `roleId` INT NULL,
  `name` TEXT NULL,
  `freqId` INT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  INDEX `uavTypeId` (`uavTypeId` ASC),
  INDEX `id` (`id` ASC),
  CONSTRAINT `uavTypeId`
    FOREIGN KEY (`uavTypeId`)
    REFERENCES `UAV`.`UAVTypes` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `IUAVROLE`
    FOREIGN KEY (`roleId`)
    REFERENCES `UAV`.`UAVRoles` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAV`.`DeviceTypes`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`DeviceTypes` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAV`.`Devices`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`Devices` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `deviceTypeId` INT NULL,
  `port` MEDIUMINT NULL,
  `uavId` INT NULL,
  PRIMARY KEY (`id`),
  INDEX `uavId` (`uavId` ASC),
  INDEX `deviceTypeId` (`deviceTypeId` ASC),
  CONSTRAINT `device`
    FOREIGN KEY (`deviceTypeId`)
    REFERENCES `UAV`.`DeviceTypes` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

-- -----------------------------------------------------
-- Table `UAV`.`SourceTypes`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`SourceTypes` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAV`.`Sources`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`Sources` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `sourceTypeId` INT NULL,
  `sourceId` INT NULL,
  PRIMARY KEY (`id`),
  INDEX `sourceTypeId` (`sourceTypeId` ASC),
  CONSTRAINT `source`
   FOREIGN KEY (`sourceTypeId`)
   REFERENCES `UAV`.`SourceTypes` (`id`)
   ON DELETE NO ACTION
   ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAV`.`Status`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`StatusTypes` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAV`.`Info`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`Info` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `uavID` INT NULL,
  `device` INT NULL,
  `source` INT NULL,
  `uavName` TEXT NULL,
  `latitude` DOUBLE NOT NULL,
  `longitude` DOUBLE NOT NULL,
  `altitude` DOUBLE NULL,
  `speed` DOUBLE NULL,
  `yaw` DOUBLE NULL,
  `restTime` TIME NULL,
  `statusTypeId` INT NULL,
  `datetime` DATETIME NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `IUAVID` (`uavID` ASC),
  INDEX `deviceID` (`device` ASC),
  INDEX `st` (`statusTypeId` ASC),
  CONSTRAINT `deviceID`
    FOREIGN KEY (`device`)
    REFERENCES `UAV`.`Devices` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `sourceID`
    FOREIGN KEY (`source`)
    REFERENCES `UAV`.`Sources` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `IUAVID`
    FOREIGN KEY (`uavID`)
    REFERENCES `UAV`.`UAV` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `stID`
    FOREIGN KEY (`statusTypeId`)
    REFERENCES `UAV`.`StatusTypes` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAV`.`TargetType`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`TargetTypes` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `id` (`id` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAV`.`Target`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`Target` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `uavID` INT NULL,
  `ip` TEXT NULL,
  `port` MEDIUMINT NULL,
--  `targetId` INT NULL, WHY?!
  `targetTypeId` INT NULL,
  PRIMARY KEY (`id`),
  INDEX `TtypeID` (`targetTypeId` ASC),
  CONSTRAINT `TtypeID`
    FOREIGN KEY (`targetTypeId`)
    REFERENCES `UAV`.`TargetTypes` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `TARGETUAVID`
    FOREIGN KEY (`uavID`)
    REFERENCES `UAV`.`UAV` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAV`.`UAVMission`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAV`.`UAVMission` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `uavID` INT NULL,
  `targetID` INT NULL,
  `regionCenterLatitude` DOUBLE NULL,
  `regionCenterLongitude` DOUBLE NULL,
  `regionCenterAltitude` DOUBLE NULL,
  `regionRadius` DOUBLE NULL,
  `timeToTarget` TIME NULL,
  PRIMARY KEY (`id`),
  INDEX `UAVid` (`uavID` ASC),
  INDEX `targetId` (`targetId` ASC),
  CONSTRAINT `UAVID`
    FOREIGN KEY (`uavID`)
    REFERENCES `UAV`.`UAV` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `targetId`
    FOREIGN KEY (`targetId`)
    REFERENCES `UAV`.`Target` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

-- -----------------------------------------------------
-- CREATE USER FOR A "testStations" DATABASE
-- -----------------------------------------------------
GRANT ALL ON `stations`.* TO 'uav'@'localhost' IDENTIFIED BY 'qwerty12345';