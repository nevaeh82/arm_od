SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `BLA` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `BLA` ;

-- -----------------------------------------------------
-- Table `BLA`.`TypeBLADictionary`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLA`.`TypeBLADictionary` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `typeID` (`id` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLA`.`BLA`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLA`.`BLA` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `idBLA` INT UNSIGNED NOT NULL,
  `ip` TEXT NOT NULL,
  `type` INT NULL,
  `name` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `typeID` (`type` ASC),
  INDEX `id` (`id` ASC),
  CONSTRAINT `typeID`
    FOREIGN KEY (`type`)
    REFERENCES `BLA`.`TypeBLADictionary` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLA`.`DeviceDictionary`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLA`.`DeviceDictionary` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLA`.`Devices`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLA`.`Devices` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `deviceID` INT NULL,
  `port` MEDIUMINT NULL,
  `BLAID` INT NULL,
  PRIMARY KEY (`id`),
  INDEX `BLAID` (`BLAID` ASC),
  INDEX `deviceID` (`deviceID` ASC),
  CONSTRAINT `device`
    FOREIGN KEY (`deviceID`)
    REFERENCES `BLA`.`DeviceDictionary` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLA`.`StatusDictionary`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLA`.`StatusDictionary` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLA`.`Info`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLA`.`Info` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `BLA_id` INT NULL,
  `device` INT NULL,
  `latitude` DOUBLE NOT NULL,
  `longitude` DOUBLE NOT NULL,
  `altitude` DOUBLE NULL,
  `speed` DOUBLE NULL,
  `yaw` DOUBLE NULL,
  `restTime` TIME NULL,
  `statusID` INT NULL,
  `datetime` DATETIME NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `IBLAID` (`BLA_id` ASC),
  INDEX `deviceID` (`device` ASC),
  INDEX `st` (`statusID` ASC),
  CONSTRAINT `deviceID`
    FOREIGN KEY (`device`)
    REFERENCES `BLA`.`Devices` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `IBLAID`
    FOREIGN KEY (`BLA_id`)
    REFERENCES `BLA`.`BLA` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `stID`
    FOREIGN KEY (`statusID`)
    REFERENCES `BLA`.`StatusDictionary` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLA`.`TargetTypeDictionary`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLA`.`TargetTypeDictionary` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `id` (`id` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLA`.`Target`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLA`.`Target` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `ip` TEXT NULL,
  `port` MEDIUMINT NULL,
  `targetID` INT NULL,
  `type` INT NULL,
  PRIMARY KEY (`id`),
  INDEX `TtypeID` (`type` ASC),
  CONSTRAINT `TtypeID`
    FOREIGN KEY (`type`)
    REFERENCES `BLA`.`TargetTypeDictionary` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `BLA`.`BLAMission`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BLA`.`BLAMission` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `BLAid` INT NULL,
  `targetID` INT NULL,
  `regionCenterLatitude` DOUBLE NULL,
  `regionCenterLongitude` DOUBLE NULL,
  `regionCenterAtitude` DOUBLE NULL,
  `regionRadius` DOUBLE NULL,
  `timeToTarget` TIME NULL,
  PRIMARY KEY (`id`),
  INDEX `BLAid` (`BLAid` ASC),
  INDEX `targetID` (`targetID` ASC),
  CONSTRAINT `BLAID`
    FOREIGN KEY (`BLAid`)
    REFERENCES `BLA`.`BLA` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `targetID`
    FOREIGN KEY (`targetID`)
    REFERENCES `BLA`.`Target` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
