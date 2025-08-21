.grinn-make {
    USE CMD({makefont}, FILE {\*}, FILE-ONCREATE {GrinnMoji.ttf}, SPECIAL ARGUMENTS {--create-ttf}, {--fast-bulid}); // Creates an empty font
    USE CMD({mount.font}, FILE {GrinnMoji.ttf}, ADD-CODE {U+1F604, U+1FAE9, U+1F92A, U+1F60E, U+1F525, U+1F60A, U+1F619, U+1F92F} // Put the Unicode of each one, except for Distorted Face and 😄with ✌️
}