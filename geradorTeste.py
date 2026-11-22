class SnapshotAncestral:
    def __init__(
        self,
        registrado=False,
        id_animal="",
        sexo=" ",
        ultimo_peso=0.0,
        status="",
        snapshot_pai=None,
        snapshot_mae=None,
    ):
        self.registrado = registrado
        self.id_animal = id_animal
        self.sexo = sexo
        self.ultimo_peso = ultimo_peso
        self.status = status
        self.snapshot_pai = snapshot_pai
        self.snapshot_mae = snapshot_mae

    @staticmethod
    def _codificar(texto):
        if not texto:
            return "_"
        return texto.replace(" ", "_")

    def serializar(self):
        reg = "1" if self.registrado else "0"
        id_fmt = self._codificar(self.id_animal)
        sexo_fmt = "_" if not self.sexo or self.sexo == " " else self.sexo
        peso_fmt = f"{self.ultimo_peso:.2f}"
        status_fmt = self._codificar(self.status)

        tokens = [reg, id_fmt, sexo_fmt, peso_fmt, status_fmt]

        if self.snapshot_pai is not None and self.snapshot_pai.registrado:
            tokens.append("1")
            tokens.append(self.snapshot_pai.serializar())
        else:
            tokens.append("0")

        if self.snapshot_mae is not None and self.snapshot_mae.registrado:
            tokens.append("1")
            tokens.append(self.snapshot_mae.serializar())
        else:
            tokens.append("0")

        return " ".join(tokens)


def formatar_genealogia(id_pai="", id_mae="", snap_pai=None, snap_mae=None):
    pai_fmt = id_pai if id_pai else "_"
    mae_fmt = id_mae if id_mae else "_"
    s_pai = snap_pai if snap_pai is not None else SnapshotAncestral()
    s_mae = snap_mae if snap_mae is not None else SnapshotAncestral()
    return f"{pai_fmt} {mae_fmt} {s_pai.serializar()} {s_mae.serializar()}"


def gerar_teste_gigante(caminho_arquivo="teste_gigante.txt"):
    contador_id = 1
    vacas_registradas = []
    touros_registrados = []

    avo_paterno_elite = SnapshotAncestral(True, "TR_ANC_01", "M", 920.0, "Ativo")
    avo_materno_elite = SnapshotAncestral(True, "VC_ANC_01", "F", 580.0, "Ativo")

    with open(caminho_arquivo, "w", encoding="utf-8") as f:
        f.write("AREA Retiro_Seca seco 100.0\n")
        f.write("AREA Invernada_Ruim ruim 200.0\n")
        f.write("AREA Confinamento_Central intermediario 400.0\n")
        f.write("AREA Pasto_Leite bom 600.0\n")
        f.write("AREA Maternidade_Elite muito_bom 600.0\n")
        f.write("AREA Piquete_Touros muito_bom 100.0\n")

        for i in range(1500):
            id_vaca = f"VC{contador_id}"
            contador_id += 1
            peso = 480.0 + (i % 100)
            idade = 12 + (i % 72)
            lact = 1 if (idade >= 24 and i % 2 == 0) else 0
            litros = (15.0 + (i % 15)) if lact == 1 else 0.0

            if i % 5 == 0:
                snap_pai = avo_paterno_elite
                snap_mae = avo_materno_elite
                id_pai = avo_paterno_elite.id_animal
                id_mae = avo_materno_elite.id_animal
            else:
                snap_pai = SnapshotAncestral()
                snap_mae = SnapshotAncestral()
                id_pai = ""
                id_mae = ""

            snap_vaca = SnapshotAncestral(True, id_vaca, "F", peso, "Ativo", snap_pai, snap_mae)
            vacas_registradas.append(snap_vaca)

            gen = formatar_genealogia(id_pai, id_mae, snap_pai, snap_mae)
            f.write(f"ANIMAL Pasto_Leite vaca AUTO {peso:.2f} {idade} {lact} {litros:.2f} {gen}\n")

        for i in range(100):
            id_touro = f"TR{contador_id}"
            contador_id += 1
            peso = 650.0 + (i % 250)
            idade = 12 + (i % 60)
            dose = 50.0 + (i % 80)

            if i % 4 == 0:
                snap_pai = avo_paterno_elite
                snap_mae = avo_materno_elite
                id_pai = avo_paterno_elite.id_animal
                id_mae = avo_materno_elite.id_animal
            else:
                snap_pai = SnapshotAncestral()
                snap_mae = SnapshotAncestral()
                id_pai = ""
                id_mae = ""

            snap_touro = SnapshotAncestral(True, id_touro, "M", peso, "Ativo", snap_pai, snap_mae)
            touros_registrados.append(snap_touro)

            gen = formatar_genealogia(id_pai, id_mae, snap_pai, snap_mae)
            f.write(f"ANIMAL Piquete_Touros touro AUTO {peso:.2f} {idade} {dose:.2f} {gen}\n")

        for i in range(1000):
            contador_id += 1
            peso = 380.0 + (i % 180)
            idade = 16 + (i % 24)
            eng = i % 2

            if i % 3 == 0:
                pai = touros_registrados[i % len(touros_registrados)]
                mae = vacas_registradas[i % len(vacas_registradas)]
                gen = formatar_genealogia(pai.id_animal, mae.id_animal, pai, mae)
            else:
                gen = formatar_genealogia()

            f.write(f"ANIMAL Confinamento_Central boicorte AUTO {peso:.2f} {idade} {eng} 0.52 {gen}\n")

        for i in range(2000):
            contador_id += 1
            sexo = "M" if i % 2 == 0 else "F"
            idade = 1 + (i % 14)
            desm = 1 if idade >= 7 else 0
            peso = float((160 + (i % 90)) if desm == 1 else (45 + (i % 80)))

            pai = touros_registrados[i % len(touros_registrados)]
            mae = vacas_registradas[i % len(vacas_registradas)]
            gen = formatar_genealogia(pai.id_animal, mae.id_animal, pai, mae)

            f.write(f"ANIMAL Maternidade_Elite bezerro AUTO {sexo} {peso:.2f} {idade} {desm} {gen}\n")

        for i in range(300):
            contador_id += 1
            gen = formatar_genealogia()
            if i % 2 == 0:
                peso = float(350 + i % 100)
                idade = 18 + i % 12
                f.write(f"ANIMAL Invernada_Ruim boicorte AUTO {peso:.2f} {idade} 0 0.50 {gen}\n")
            else:
                peso = float(450 + i % 80)
                idade = 30 + i % 40
                f.write(f"ANIMAL Invernada_Ruim vaca AUTO {peso:.2f} {idade} 0 0.00 {gen}\n")

        for i in range(100):
            contador_id += 1
            if i % 2 == 0:
                peso = float(170 + i % 50)
                idade = 8 + i % 6
                pai = touros_registrados[i % len(touros_registrados)]
                mae = vacas_registradas[i % len(vacas_registradas)]
                gen = formatar_genealogia(pai.id_animal, mae.id_animal, pai, mae)
                f.write(f"ANIMAL Retiro_Seca bezerro AUTO M {peso:.2f} {idade} 1 {gen}\n")
            else:
                peso = float(340 + i % 90)
                idade = 20 + i % 15
                gen = formatar_genealogia()
                f.write(f"ANIMAL Retiro_Seca boicorte AUTO {peso:.2f} {idade} 0 0.50 {gen}\n")

    print(f"{caminho_arquivo} gerado com sucesso!")


if __name__ == "__main__":
    gerar_teste_gigante()