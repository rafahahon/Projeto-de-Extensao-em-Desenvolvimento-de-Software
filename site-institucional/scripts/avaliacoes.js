const filtros = document.querySelectorAll('input[name="filtro"]');

filtros.forEach(radio => {
    radio.addEventListener("change", e => {
        const selecionado = e.currentTarget,
            avaliacoes = document.querySelectorAll('.grade-avaliacoes .cartao-avaliacao');

        // Mostramos todas
        avaliacoes.forEach(el => {
            el.style.opacity = 1;
            el.style.display = "block";
        });

        if (selecionado.value !== "0") {
            // Escondemos a que não tem a quantia de estrelas do selecionado
            avaliacoes.forEach(el => {
                if (el.dataset.nota !== selecionado.value) {
                    el.style.opacity = 0;
                    el.style.display = "none";
                }
            });
        }
    });
});

// Testando se o navegador tem suporta ao elemento HTML template checando
// a presença do atributo content no elemento template
if ("content" in document.createElement("template")) {
    // Instanciamos as avaliações existentes e o template de cartão
    const formAvaliacao = document.querySelector("#form-avaliacao"),
        avaliacoes = document.querySelector(".grade-avaliacoes"),
        template = document.querySelector("#tmplt-avaliacao");

    formAvaliacao.addEventListener("submit", e => {
        const clone = document.importNode(template.content, true);
        let estrelas = ["☆", "☆", "☆", "☆", "☆"],
            cartao = clone.querySelector(".cartao-avaliacao"),
            nota = formAvaliacao.querySelector("input[name='estrelas']:checked"),
            nome = formAvaliacao.querySelector("input[name='nome']"),
            avaliacao = formAvaliacao.querySelector("textarea[name='avaliacao']");

        formAvaliacao.querySelector("button").disabled = true;
        formAvaliacao.querySelectorAll("input, textarea").forEach(el => {
            el.disabled = true;
        });

        for (let i = 0; i <= nota.value; i++) {
            estrelas[i] = "★";
        }

        cartao.querySelector(".nota-estrelas").textContent = estrelas.join("");
        cartao.querySelector(".autor").textContent = nome.value;
        cartao.querySelector("blockquote p").textContent = `“${avaliacao.value}”`;

        avaliacoes.appendChild(clone);

        if (parseInt(nota.value, 10) > 3) {
            alert(`Sua avaliação foi publicada! Obrigado pelas ${nota.value} estrelas!`);
        } else {
            alert(`Sua avaliação foi publicada! For favor diga-nos como podemos melhorar!`);
        }

        formAvaliacao.reset();
        formAvaliacao.querySelector("button").disabled = false;
        formAvaliacao.querySelectorAll("input, textarea").forEach(el => {
            el.disabled = false;
        });

        e.preventDefault();
        e.stopPropagation();
        return false;
    });
}
