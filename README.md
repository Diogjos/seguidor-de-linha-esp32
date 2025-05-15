# 🚗 Carro Seguidor de Linha com ESP32

Este projeto consiste em um robô seguidor de linha construído com a placa ESP32, sensores TCRT5000, ponte H (L298N) e motores DC. O robô é capaz de seguir uma faixa preta sobre fundo branco utilizando leitura analógica dos sensores.

## 📷 Imagens do Projeto

![Foto lateral do carro](./fotos/carro_lateral.jpg)
![Foto superior do carro](./fotos/carro_superior.jpg)

## 🎥 Demonstração

[![Assista no YouTube](https://img.shields.io/badge/Ver%20demo%20em%20vídeo-YouTube-red)](https://link-do-seu-video.com)

## 🧠 Objetivo

O objetivo deste projeto é demonstrar os fundamentos de robótica móvel autônoma aplicando controle simples baseado em sensores de refletância. O projeto pode ser usado em competições de robótica ou como aprendizado introdutório à eletrônica embarcada.

---

## 🛠️ Componentes Utilizados

| Componente                 | Quantidade | Descrição                              |
|---------------------------|------------|----------------------------------------|
| ESP32 DevKit              | 1          | Microcontrolador principal             |
| Sensor TCRT5000           | 5          | Sensores de linha (infravermelho)      |
| Ponte H L298N             | 1          | Controle de motores DC                 |
| Motor DC 3–6V             | 2          | Motores das rodas                      |
| Rodas                     | 2          | Acopladas nos motores                  |
| Rodízio (roda boba)       | 1          | Para estabilidade                      |
| Chassi de guarda-roupa     | 1          | Base do robô                           |
| Jumpers                  | vários     | Conexões elétricas                     |
| Fonte 7.4V ou 9V          | 1          | Alimentação                            |
| Conversor Step Down       | 1          | Redução de tensão para o ESP32         |

---

## 🔌 Esquema de Ligações

| ESP32 | Módulo / Sensor    | Descrição                          |
|-------|--------------------|------------------------------------|
| 32–36 | TCRT5000 (1 a 5)   | Entradas analógicas dos sensores   |
| 25    | ENA                | PWM motor esquerdo                 |
| 26    | IN1                | Direção motor esquerdo             |
| 27    | IN2                | Direção motor esquerdo             |
| 14    | ENB                | PWM motor direito                  |
| 12    | IN3                | Direção motor direito              |
| 13    | IN4                | Direção motor direito              |

---

## 📄 Código Fonte

O código está disponível no arquivo [`carro_seguidor.ino`](./carro_seguidor.ino). Ele realiza as seguintes funções principais:

- Leitura analógica dos 5 sensores TCRT5000.
- Decisão do movimento baseado nos sensores ativos.
- Controle de velocidade com `PWM` via `ledcWrite()`.
- Correções suaves e fortes de direção com base na posição da linha.

### Principais Funções:

- `lerSensores()` – Lê os valores dos sensores.
- `estaNaLinha(sensor)` – Verifica se um sensor está sobre a linha.
- `seguirLinha()` – Lógica de navegação principal.
- `setMotores()` – Controla os motores com PWM.

---

## 🚦 Lógica de Decisão

```text
           [S0] [S1] [S2] [S3] [S4]
             \   |   |   |   /
              \        |       /
               -- seguir linha --
