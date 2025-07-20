# TraderSim++

**High-Frequency Trading Simulator**

---

## Overview

**TraderSim++** is a high-frequency trading (HFT) simulator designed to replicate a realistic market environment. It provides a sophisticated order book and matching engine that enables testing and prototyping of algorithmic trading strategies with real-time order matching, latency modeling, and profit & loss (PnL) tracking.

---

## What It Does

- Simulates a realistic order book with price-time priority matching  
- Supports market and limit orders with partial fills and cancellations  
- Models latency and concurrency for multi-threaded strategy execution  
- Tracks positions and calculates both realized and unrealized PnL  
- Allows backtesting of trading strategies using historical tick data  
- Provides an event-driven architecture to notify subscribers of trade and order events  

---

## Why Use TraderSim++

Algorithmic trading strategy testing is often either too simplistic or requires expensive market data and infrastructure. TraderSim++ offers a risk-free, cost-effective platform to prototype and validate strategies under pseudo-realistic market conditions.

---

## Key Features and Learning Areas

- **Multithreading:** Separate exchange engine, data feed, and trading strategy threads with safe message passing  
- **Order Book Data Structures:** Efficient use of priority queues and linked lists to manage order levels and orders  
- **Performance Optimization:** Memory pool usage and cache locality considerations for low-latency processing  
- **Event-Driven Design:** Observer pattern implementation for handling trade, order, and market data events  
- **Strategy Backtesting:** Framework to simulate historical market scenarios and evaluate strategy performance  


