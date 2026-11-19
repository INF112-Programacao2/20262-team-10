with open("teste_gigante.txt", "w", encoding="utf-8") as f:
    # areas
    f.write("AREA Retiro_Seca seco 100.0\n")
    f.write("AREA Invernada_Ruim ruim 200.0\n")
    f.write("AREA Confinamento_Central intermediario 400.0\n")
    f.write("AREA Pasto_Leite bom 600.0\n")
    f.write("AREA Maternidade_Elite muito_bom 600.0\n")
    f.write("AREA Piquete_Touros muito_bom 100.0\n")

    # vacas
    for i in range(1500):
        peso = 480 + (i % 100)
        idade = 12 + (i % 72)
        lact = 1 if (idade >= 24 and i % 2 == 0) else 0
        litros = 15 + (i % 15) if lact == 1 else 0
        f.write(f"ANIMAL Pasto_Leite vaca AUTO {peso} {idade} {lact} {litros}\n")

    # touros
    for i in range(100):
        peso = 650 + (i % 250)
        idade = 12 + (i % 60)
        dose = 50 + (i % 80)
        f.write(f"ANIMAL Piquete_Touros touro AUTO {peso} {idade} {dose}\n")

    # bois de corte
    for i in range(1000):
        peso = 380 + (i % 180)
        idade = 16 + (i % 24)
        eng = i % 2
        f.write(f"ANIMAL Confinamento_Central boicorte AUTO {peso} {idade} {eng} 0.52\n")

    # bezerros
    for i in range(2000):
        sexo = "M" if i % 2 == 0 else "F"
        idade = 1 + (i % 14)
        desm = 1 if idade >= 7 else 0
        peso = (160 + (i % 90)) if desm == 1 else (45 + (i % 80))
        f.write(f"ANIMAL Maternidade_Elite bezerro AUTO {sexo} {peso} {idade} {desm}\n")

    # mistura na invernada
    for i in range(300):
        if i % 2 == 0:
            f.write(f"ANIMAL Invernada_Ruim boicorte AUTO {350 + i % 100} {18 + i % 12} 0 0.50\n")
        else:
            f.write(f"ANIMAL Invernada_Ruim vaca AUTO {450 + i % 80} {30 + i % 40} 0 0\n")

    # mistura no retiro
    for i in range(100):
        if i % 2 == 0:
            f.write(f"ANIMAL Retiro_Seca bezerro AUTO M {170 + i % 50} {8 + i % 6} 1\n")
        else:
            f.write(f"ANIMAL Retiro_Seca boicorte AUTO {340 + i % 90} {20 + i % 15} 0 0.50\n")

print("teste_gigante.txt gerado!")