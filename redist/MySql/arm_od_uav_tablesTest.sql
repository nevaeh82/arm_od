SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `UAVTEST` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `UAVTEST` ;

-- -----------------------------------------------------
-- Table `UAVTEST`.`UAVTypes`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAVTEST`.`UAVTypes` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `typeId` (`id` ASC))
ENGINE = InnoDB;

-- -----------------------------------------------------
-- Table `UAVTEST`.`UAVRoles`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAVTEST`.`UAVRoles` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `code` TEXT NOT NULL,
  `name` TEXT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `roleId` (`id` ASC))
ENGINE = InnoDB;

-- -----------------------------------------------------
-- Table `UAVTEST`.`UAV`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAVTEST`.`UAV` (
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
    REFERENCES `UAVTEST`.`UAVTypes` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `IUAVROLE`
    FOREIGN KEY (`roleId`)
    REFERENCES `UAVTEST`.`UAVRoles` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAVTEST`.`DeviceTypes`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAVTEST`.`DeviceTypes` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAVTEST`.`Devices`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAVTEST`.`Devices` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `deviceTypeId` INT NULL,
  `port` MEDIUMINT NULL,
  `uavId` INT NULL,
  PRIMARY KEY (`id`),
  INDEX `uavId` (`uavId` ASC),
  INDEX `deviceTypeId` (`deviceTypeId` ASC),
  CONSTRAINT `device`
    FOREIGN KEY (`deviceTypeId`)
    REFERENCES `UAVTEST`.`DeviceTypes` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAVTEST`.`Status`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAVTEST`.`StatusTypes` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAVTEST`.`Info`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAVTEST`.`Info` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `uavID` INT NULL,
  `device` INT NULL,
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
    REFERENCES `UAVTEST`.`Devices` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `IUAVID`
    FOREIGN KEY (`uavID`)
    REFERENCES `UAVTEST`.`UAV` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `stID`
    FOREIGN KEY (`statusTypeId`)
    REFERENCES `UAVTEST`.`StatusTypes` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAVTEST`.`TargetType`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAVTEST`.`TargetTypes` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `id` (`id` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAVTEST`.`Target`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAVTEST`.`Target` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `ip` TEXT NULL,
  `port` MEDIUMINT NULL,
--  `targetId` INT NULL, WHY?!
  `targetTypeId` INT NULL,
  PRIMARY KEY (`id`),
  INDEX `TtypeID` (`targetTypeId` ASC),
  CONSTRAINT `TtypeID`
    FOREIGN KEY (`targetTypeId`)
    REFERENCES `UAVTEST`.`TargetTypes` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `UAVTEST`.`UAVMission`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `UAVTEST`.`UAVMission` (
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
    REFERENCES `UAVTEST`.`UAV` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `targetId`
    FOREIGN KEY (`targetId`)
    REFERENCES `UAVTEST`.`Target` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
