-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Хост: 127.0.0.1
-- Время создания: Янв 24 2024 г., 00:11
-- Версия сервера: 10.4.27-MariaDB
-- Версия PHP: 8.2.0

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- База данных: `users`
--

-- --------------------------------------------------------

--
-- Структура таблицы `userhistory`
--

CREATE TABLE `userhistory` (
  `user` varchar(30) NOT NULL,
  `MAC` varchar(30) NOT NULL,
  `task` varchar(30) NOT NULL,
  `inputs` varchar(30) NOT NULL,
  `results` varchar(30) NOT NULL,
  `time` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Дамп данных таблицы `userhistory`
--

INSERT INTO `userhistory` (`user`, `MAC`, `task`, `inputs`, `results`, `time`) VALUES
('test', 'ip', '1', '133', '7', 'now'),
('test', 'ip', '1', '123', '6', 'Sun Mar 19 10:26:26 2023'),
('test', '0x8030491b77d2', '1', '123', '6', 'Sun Mar 19 10:29:37 2023'),
('Ilyas', '0x8030491b77d2', '1', '444', '12', 'Sun Mar 19 10:47:58 2023'),
('Ilyas', '0x8030491b77d2', '1', '777', '21', 'Sun Mar 19 12:27:26 2023'),
('Ilyas', '0x8030491b77d2', '2', '1355', '403', 'Sun Mar 19 13:11:56 2023'),
('Ilyas', '0x8030491b77d2', '2', '13;55', '398', 'Sun Mar 19 13:12:28 2023'),
('Ilyas', '0x8030491b77d2', '1', '888', '24', 'Sun Mar 19 13:13:13 2023'),
('Assankhan', '0x8030491b77d2', '1', '123', '6', 'Sun Mar 19 13:13:40 2023'),
('Assankhan', '0x8030491b77d2', '2', '2;34', '48', 'Sun Mar 19 13:13:48 2023'),
('Assankhan', '0x8030491b77d2', '2', '2;34', '52', 'Sun Mar 19 13:13:51 2023'),
('Assankhan', '0x8030491b77d2', '1', '133', '7', 'Sun Mar 19 13:13:59 2023'),
('Assankhan', '0x8030491b77d2', '1', '133', '7', 'Sun Mar 19 13:14:01 2023'),
('Assankhan', '0x8030491b77d2', '1', '133', '7', 'Sun Mar 19 13:14:03 2023'),
('Assankhan', '0x8030491b77d2', '2', '3;3', '6', 'Sun Mar 19 13:17:44 2023'),
('Assankhan', '0x8030491b77d2', '2', '3;3', '7', 'Sun Mar 19 13:18:09 2023'),
('Assankhan', '0x8030491b77d2', '2', '3;3', '6', 'Sun Mar 19 13:18:56 2023'),
('Assankhan', '0x8030491b77d2', '1', '123', '6', 'Sun Mar 19 13:19:09 2023'),
('Assankhan', '0x8030491b77d2', '2', '4;4', '7', 'Sun Mar 19 13:19:57 2023'),
('Assankhan', '0x8030491b77d2', '2', '5;5', '17', 'Sun Mar 19 13:21:03 2023'),
('Assankhan', '0x8030491b77d2', '1', '445', '13', 'Sun Mar 19 13:21:20 2023'),
('Ilyas', '0x8030491b77d2', '1', '123', '6', 'Mon Mar 20 01:05:37 2023'),
('Ilyas', '0x8030491b77d2', '1', '123', '6', 'Mon Mar 20 01:05:46 2023'),
('Ilyas', '0x8030491b77d2', '1', '123', '6', 'Mon Mar 20 01:06:48 2023'),
('Ilyas', '0x8030491b77d2', '1', '123', '6', 'Mon Mar 20 01:07:02 2023'),
('Ilyas', '0x8030491b77d2', '1', '123', '6', 'Mon Mar 20 01:08:00 2023'),
('Ilyas', '0x8030491b77d2', '1', '4444', '16', 'Mon Mar 20 01:08:24 2023'),
('Ilyas', '0x8030491b77d2', '2', '12;12', '9', 'Mon Mar 20 01:09:08 2023'),
('Ilyas', '0x8030491b77d2', '2', '12;12', '89', 'Mon Mar 20 01:10:06 2023'),
('Ilyas', '0x8030491b77d2', '2', '12;12', '69', 'Mon Mar 20 01:11:20 2023'),
('Ilyas', '0x8030491b77d2', '2', '11;11', '48', 'Mon Mar 20 01:13:04 2023'),
('Ilyas', '0x8030491b77d2', '1', '4445', '17', 'Mon Mar 20 01:13:45 2023'),
('Ilyas', '0x8030491b77d2', '1', '4445', '17', 'Mon Mar 20 01:16:54 2023'),
('Ilyas', '0x8030491b77d2', '1', '4445', '17', 'Mon Mar 20 01:16:58 2023'),
('Ilyas', '0x8030491b77d2', '1', '4445', '17', 'Mon Mar 20 01:17:05 2023'),
('Ilyas', '0x8030491b77d2', '1', '4445', '17', 'Mon Mar 20 01:17:12 2023'),
('Ilyas', '0x8030491b77d2', '2', '11;11', '54', 'Mon Mar 20 01:19:38 2023'),
('Ilyas', '0x8030491b77d2', '1', '3333', '12', 'Mon Mar 20 01:20:47 2023'),
('Ilyas', '0x8030491b77d2', '1', '222222', '12', 'Mon Mar 20 01:24:36 2023'),
('Ilyas', '0x8030491b77d2', '1', '222222', '12', 'Mon Mar 20 01:24:42 2023'),
('Ilyas', '0x8030491b77d2', '1', '222222', '12', 'Mon Mar 20 01:25:58 2023'),
('Ilyas', '0x8030491b77d2', '1', '222222', '12', 'Mon Mar 20 01:27:26 2023'),
('Ilyas', '0x8030491b77d2', '1', '222222', '12', 'Mon Mar 20 01:28:50 2023'),
('Ilyas', '0x8030491b77d2', '1', '222222', '12', 'Mon Mar 20 01:29:03 2023'),
('ertutu', '0x8030491b77d1', '2', '14;99', '694', 'Mon Mar 20 10:18:45 2023'),
('ertutu', '0x8030491b77d1', '1', '7890', '24', 'Mon Mar 20 10:19:25 2023'),
('ertutu', '0x8030491b77d1', '2', '3;3', '7', 'Mon Mar 20 10:22:46 2023'),
('Nuraly', '0x5056c00008', '1', '234567890', '44', 'Fri Mar 24 01:26:36 2023'),
('Nuraly', '0x8030491b77d2', '1', '123', '6', 'Wed Jan 24 04:08:11 2024'),
('Nuraly', '0x8030491b77d2', '2', '12;55', '377', 'Wed Jan 24 04:08:26 2024');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
