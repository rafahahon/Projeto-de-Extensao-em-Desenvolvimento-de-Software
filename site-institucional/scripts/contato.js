const formContato = document.querySelector("#form-contato");
formContato.addEventListener("submit", e => {
    const formContato = e.currentTarget,
        botaoEnviar = formContato.querySelector("button"),
        campos = formContato.querySelectorAll("input, textarea");

    botaoEnviar.disabled = true;
    campos.forEach(el => {
        el.disabled = true;
    });

    alert("Obrigado por ter entrado em contato conosco! Em breve retornaremos o contato.");

    formContato.reset();
    botaoEnviar.disabled = false;
    campos.forEach(el => {
        el.disabled = false;
    });

    e.preventDefault();
    e.stopPropagation();
    return false;
});
