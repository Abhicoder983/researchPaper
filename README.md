# A Lightweight ID-less Authentication Mechanism for IoT–UAV Communication

## 📌 Project Title
A Lightweight ID-less Authentication Mechanism for IoT–UAV Communication Using Timestamp Division and HMAC

---

## ⚙️ Specifications
- **Simulation Tool:** ns-3  
- **Programming Language:** C++  
- **Network Standard:** IEEE 802.11b (Ad hoc mode)  
- **Communication Protocol:** UDP  
- **Number of IoT Devices:** Configurable (1–50)  
- **UAV Nodes:** 1 (Edge/MEC node)  
- **Packet Size:** ~190 bytes  
- **Authentication Delay:** ~13 ms  
- **Simulation Time:** 10 seconds  

---

## 📖 Introduction
The rapid growth of IoT devices and UAV-based edge computing introduces significant security challenges in wireless communication. Traditional authentication methods either expose device identity or require heavy computation.

This project proposes a lightweight ID-less authentication mechanism that eliminates the need to transmit device identity. It uses SHA-256 hashing, timestamp-based division, and HMAC-SHA256 to ensure privacy, security, and low-latency authentication suitable for resource-constrained IoT devices.

---

## 🛠️ Implementation
The system is implemented using the ns-3 network simulator to model real-world IoT–UAV communication.

- Multiple IoT devices are created as nodes in ns-3  
- A UAV node acts as the authentication verifier (edge node)  
- Communication is established using UDP over IEEE 802.11b  
- Authentication is simulated using a fixed delay (~13 ms)  
- Packet size is kept lightweight (~190 bytes)  
- The simulation evaluates network delay and authentication performance  

### ▶️ Steps to Run
1. Place the file in:

2. Build ns-3:

3. Run simulation:

4. Run with custom devices:


---

## 👨‍💻 Authors
- Abhishek Kumar  
- Team Members  

---

## 📄 License
This project is for academic and research purposes only.