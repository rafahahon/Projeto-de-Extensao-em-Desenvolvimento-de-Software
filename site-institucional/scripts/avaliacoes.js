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
